/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See License.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
};
var __param = (this && this.__param) || function (paramIndex, decorator) {
    return function (target, key) { decorator(target, key, paramIndex); }
};
define(["require", "exports", "vs/base/browser/dom", "vs/workbench/contrib/debug/common/debug", "vs/workbench/contrib/debug/common/debugModel", "vs/platform/contextview/browser/contextView", "vs/base/browser/ui/inputbox/inputBox", "vs/base/common/lifecycle", "vs/platform/theme/common/themeService", "vs/platform/theme/common/styler", "vs/base/browser/ui/highlightedlabel/highlightedLabel", "vs/base/common/filters"], function (require, exports, dom, debug_1, debugModel_1, contextView_1, inputBox_1, lifecycle_1, themeService_1, styler_1, highlightedLabel_1, filters_1) {
    "use strict";
    Object.defineProperty(exports, "__esModule", { value: true });
    exports.MAX_VALUE_RENDER_LENGTH_IN_VIEWLET = 1024;
    exports.twistiePixels = 20;
    const booleanRegex = /^true|false$/i;
    const stringRegex = /^(['"]).*\1$/;
    const $ = dom.$;
    function renderViewTree(container) {
        const treeContainer = document.createElement('div');
        dom.addClass(treeContainer, 'debug-view-content');
        container.appendChild(treeContainer);
        return treeContainer;
    }
    exports.renderViewTree = renderViewTree;
    function replaceWhitespace(value) {
        const map = { '\n': '\\n', '\r': '\\r', '\t': '\\t' };
        return value.replace(/[\n\r\t]/g, char => map[char]);
    }
    exports.replaceWhitespace = replaceWhitespace;
    function renderExpressionValue(expressionOrValue, container, options) {
        let value = typeof expressionOrValue === 'string' ? expressionOrValue : expressionOrValue.value;
        // remove stale classes
        container.className = 'value';
        // when resolving expressions we represent errors from the server as a variable with name === null.
        if (value === null || ((expressionOrValue instanceof debugModel_1.Expression || expressionOrValue instanceof debugModel_1.Variable) && !expressionOrValue.available)) {
            dom.addClass(container, 'unavailable');
            if (value !== debugModel_1.Expression.DEFAULT_VALUE) {
                dom.addClass(container, 'error');
            }
        }
        else if ((expressionOrValue instanceof debugModel_1.ExpressionContainer) && options.showChanged && expressionOrValue.valueChanged && value !== debugModel_1.Expression.DEFAULT_VALUE) {
            // value changed color has priority over other colors.
            container.className = 'value changed';
            expressionOrValue.valueChanged = false;
        }
        if (options.colorize && typeof expressionOrValue !== 'string') {
            if (expressionOrValue.type === 'number' || expressionOrValue.type === 'boolean' || expressionOrValue.type === 'string') {
                dom.addClass(container, expressionOrValue.type);
            }
            else if (!isNaN(+value)) {
                dom.addClass(container, 'number');
            }
            else if (booleanRegex.test(value)) {
                dom.addClass(container, 'boolean');
            }
            else if (stringRegex.test(value)) {
                dom.addClass(container, 'string');
            }
        }
        if (options.maxValueLength && value && value.length > options.maxValueLength) {
            value = value.substr(0, options.maxValueLength) + '...';
        }
        if (value && !options.preserveWhitespace) {
            container.textContent = replaceWhitespace(value);
        }
        else {
            container.textContent = value || '';
        }
        if (options.showHover) {
            container.title = value || '';
        }
    }
    exports.renderExpressionValue = renderExpressionValue;
    function renderVariable(variable, data, showChanged, highlights) {
        if (variable.available) {
            let text = replaceWhitespace(variable.name);
            if (variable.value && typeof variable.name === 'string') {
                text += ':';
            }
            data.label.set(text, highlights, variable.type ? variable.type : variable.name);
            dom.toggleClass(data.name, 'virtual', !!variable.presentationHint && variable.presentationHint.kind === 'virtual');
        }
        else if (variable.value && typeof variable.name === 'string') {
            data.label.set(':');
        }
        renderExpressionValue(variable, data.value, {
            showChanged,
            maxValueLength: exports.MAX_VALUE_RENDER_LENGTH_IN_VIEWLET,
            preserveWhitespace: false,
            showHover: true,
            colorize: true
        });
    }
    exports.renderVariable = renderVariable;
    let AbstractExpressionsRenderer = class AbstractExpressionsRenderer {
        constructor(debugService, contextViewService, themeService) {
            this.debugService = debugService;
            this.contextViewService = contextViewService;
            this.themeService = themeService;
        }
        renderTemplate(container) {
            const expression = dom.append(container, $('.expression'));
            const name = dom.append(expression, $('span.name'));
            const value = dom.append(expression, $('span.value'));
            const label = new highlightedLabel_1.HighlightedLabel(name, false);
            const inputBoxContainer = dom.append(expression, $('.inputBoxContainer'));
            const toDispose = [];
            const enableInputBox = (expression, options) => {
                name.style.display = 'none';
                value.style.display = 'none';
                inputBoxContainer.style.display = 'initial';
                const inputBox = new inputBox_1.InputBox(inputBoxContainer, this.contextViewService, {
                    placeholder: options.placeholder,
                    ariaLabel: options.ariaLabel
                });
                const styler = styler_1.attachInputBoxStyler(inputBox, this.themeService);
                inputBox.value = replaceWhitespace(options.initialValue);
                inputBox.focus();
                inputBox.select();
                let disposed = false;
                toDispose.push(inputBox);
                toDispose.push(styler);
                const wrapUp = (renamed) => {
                    if (!disposed) {
                        disposed = true;
                        this.debugService.getViewModel().setSelectedExpression(undefined);
                        options.onFinish(inputBox.value, renamed);
                        // need to remove the input box since this template will be reused.
                        inputBoxContainer.removeChild(inputBox.element);
                        name.style.display = 'initial';
                        value.style.display = 'initial';
                        inputBoxContainer.style.display = 'none';
                        lifecycle_1.dispose(toDispose);
                    }
                };
                toDispose.push(dom.addStandardDisposableListener(inputBox.inputElement, 'keydown', (e) => {
                    const isEscape = e.equals(9 /* Escape */);
                    const isEnter = e.equals(3 /* Enter */);
                    if (isEscape || isEnter) {
                        e.preventDefault();
                        e.stopPropagation();
                        wrapUp(isEnter);
                    }
                }));
                toDispose.push(dom.addDisposableListener(inputBox.inputElement, 'blur', () => {
                    wrapUp(true);
                }));
                toDispose.push(dom.addDisposableListener(inputBox.inputElement, 'click', e => {
                    // Do not expand / collapse selected elements
                    e.preventDefault();
                    e.stopPropagation();
                }));
            };
            return { expression, name, value, label, enableInputBox, inputBoxContainer, toDispose };
        }
        renderElement(node, index, data) {
            const { element } = node;
            if (element === this.debugService.getViewModel().getSelectedExpression()) {
                data.enableInputBox(element, this.getInputBoxOptions(element));
            }
            else {
                this.renderExpression(element, data, filters_1.createMatches(node.filterData));
            }
        }
        disposeTemplate(templateData) {
            lifecycle_1.dispose(templateData.toDispose);
        }
    };
    AbstractExpressionsRenderer = __decorate([
        __param(0, debug_1.IDebugService),
        __param(1, contextView_1.IContextViewService),
        __param(2, themeService_1.IThemeService)
    ], AbstractExpressionsRenderer);
    exports.AbstractExpressionsRenderer = AbstractExpressionsRenderer;
});
//# sourceMappingURL=baseDebugView.js.map