/**
 * Copyright (c) Microsoft Corporation.
 * Licensed under the MIT License.
 * @format
 */

'use strict';

const React = require('react');
const ReactNative = require('react-native');

const {AppRegistry, StyleSheet, Text, View} = ReactNative;

const {TestModule} = ReactNative.NativeModules;

const URL_BASE = 'ws://localhost:9001';
const agent = 'rnw';

var statusWs = null;
var caseWs = null;

class WebSocketAutobahnTest extends React.Component {

  updateStatus(message) {
    console.log(message);
  }

  updateReports() {
    var url = URL_BASE + `/updateReports?agnet=${agent}`;
    statusWs = new WebSocket(url);
    statusWs.onopen = (e) => {
      this.updateStatus('Updating reports...')
    }
    statusWs.onclose = (e) => {
      statusWs = null;
      this.updateStatus('Reports updated');
    }
  }

  runCase(id) {
    console.log('runcase0');
    var url = URL_BASE + `/runCase?case=${id}&agent=${agent}`;

    console.log('runcase_constrING');
    testWs = new WebSocket(url);
    console.log('runcase_constrED');
    testWs.binaryType = 'arraybuffer';
    testWs.onopen = (e) => {
      console.log('runcase_onopen');
      this.updateStatus(`Executing test case ${id}`);
    }
    testWs.onclose = (e) => {
      console.log('runcase_onclsoe');
      testWs = null;

      this.updateStatus('Case socket finished');
      this.updateReports();
    }
    testWs.onerror = (e) => {
      console.log('runcase_onerror');
      TestModule.markTestPassed(false);
    }
    testWs.onmessage = (e) => {
      console.log('runcase_onmessage');
      testWs.send(e.data);
      TestModule.markTestPassed(true);
    }
  }

  componentDidMount() {
    console.log('chaaaaale');
    this.runCase(1);
    //TestModule.markTestPassed(true);
  }

  render() {
    return (
      <View style={styles.container}>
        <Text style={styles.row}>Some text</Text>
      </View>
    );
  }
}

var styles = StyleSheet.create({
  container: {},
  row: {},
});

AppRegistry.registerComponent('WebSocketAutobahnTest', () => WebSocketAutobahnTest);

module.exports = WebSocketAutobahnTest;
