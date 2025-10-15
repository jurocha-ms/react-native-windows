/**
 * Copyright (c) Microsoft Corporation.
 * Licensed under the MIT License.
 * @format
 * @flow
 */
'use strict';

const React = require('react');
const ReactNative = require('react-native');

const {AppRegistry, View} = ReactNative;

const {TestModule} = ReactNative.NativeModules;

type State = {
  statusCode: number,
  xhr: XMLHttpRequest,
};

class XHRTest extends React.Component<{...}, State> {
  state: State = {
    statusCode: 0,
    xhr: new XMLHttpRequest(),
  };

  _get = () => {
    this.state.xhr.onloadend = () => {
      this.setState({
        statusCode: this.state.xhr.status,
      });
    };
    this.state.xhr.open(
      'POST',
      'http://localhost:5555/officedev/office-js/issues/5869',
    );

    const body = new FormData();
    body.append('title', 'foo');//Test fails if body has no fields. Comment to fail the test.
    this.state.xhr.send(body);
  };

  _getSucceeded = (): boolean => {
    console.log(
      `_getSucceeded [${this.state.statusCode}],[${this.state.xhr.responseText.length}]`,
    );
    return (
      this.state.statusCode === 200
      //  &&
      // this.state.xhr.responseText.length === 387
    );
  };

  _waitFor = (condition: any, timeout: any, callback: any) => {
    let remaining = timeout;
    const timeoutFunction = function () {
      if (condition()) {
        callback(true);
        return;
      }
      remaining--;
      if (remaining === 0) {
        callback(false);
      } else {
        setTimeout(timeoutFunction, 1000);
      }
    };
    setTimeout(timeoutFunction, 1000);
  };

  componentDidMount() {
    this._get();
    this._waitFor(this._getSucceeded, 5, doneSucceeded => {
      TestModule.markTestPassed(doneSucceeded);
    });
  }

  render(): React.Node {
    return <View />;
  }
}

AppRegistry.registerComponent('XHRTest', () => XHRTest);

module.exports = XHRTest;
