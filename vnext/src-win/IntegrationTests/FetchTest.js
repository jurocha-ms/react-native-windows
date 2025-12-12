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
  'https://jsonplaceholder.typicode.com/posts/1';
const expectedContent =
  '{\n' +
  '  "userId": 1,\n' +
  '  "id": 1,\n' +
  '  "title": "sunt aut facere repellat provident occaecati excepturi optio reprehenderit",\n' +
  '  "body": "quia et suscipit\\nsuscipit recusandae consequuntur expedita et cum\\nreprehenderit molestiae ut ut quas totam\\nnostrum rerum est autem sunt rem eveniet architecto"\n' +
  '}';

type State = {
  content: string,
};

class FetchTest extends React.Component<{...}, State> {
  state: State = {
    content: '',
  };

  async componentDidMount() {
    const headers = {
      Authorization: "test",
    };
    const response = await fetch(uri, {headers});
    if (! response.ok) {
      TestModule.markTestPassed(false);
      return;
    }
    const text = await response.text();
    this.setState({content: text});

    TestModule.markTestPassed(this.state.content === expectedContent);
  }

  render(): React.Node {
    return <View />;
  }
}

AppRegistry.registerComponent('FetchTest', () => FetchTest);

module.exports = FetchTest;
