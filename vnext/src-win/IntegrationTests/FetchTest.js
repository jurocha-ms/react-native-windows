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

const uri =
  'http://localhost:5555/rnw/http/encoding/utf-8';
  //'http://localhost:5555/static/utf-8.txt';
const expectedContent = 'Property of Contoso–Amélior™';

type State = {
  uri: string,
  content: string,
};

class FetchTest extends React.Component<{...}, State> {
  state: State = {
    content: '',
  };

  async componentDidMount() {
    const response = await fetch(uri);
    const text = await response.text();
    this.setState({content: text});

    if (this.state.content === expectedContent) {
      TestModule.markTestPassed(true);
    } else {
      TestModule.markTestPassed(false);
    }
  }

  render(): React.Node {
    return <View />;
  }
}

AppRegistry.registerComponent('FetchTest', () => FetchTest);

module.exports = FetchTest;
