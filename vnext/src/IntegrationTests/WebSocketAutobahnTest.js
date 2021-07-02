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
var countWs = null;
var caseCount = 0;

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
    console.log(`ze url: [${url}]`);

    console.log('runcase_constrING');
    caseWs = new WebSocket(url);
    console.log('runcase_constrED');
    caseWs.binaryType = 'arraybuffer';
    caseWs.onopen = (e) => {
      console.log('runcase_onopen');
      this.updateStatus(`Executing test case ${id}`);
    }
    caseWs.onclose = (e) => {
      console.log('runcase_onclsoe');
      caseWs = null;

      this.updateStatus('Case socket finished');
      this.updateReports();
    }
    caseWs.onerror = (e) => {
      console.log('runcase_onerror');
      TestModule.markTestPassed(false);
    }
    caseWs.onmessage = (e) => {
      console.log('runcase_onmessage');
      caseWs.send(e.data);
      TestModule.markTestPassed(true);
    }
  }

  waitFor(condition, timeout, callback) {
    let remaining = timeout;
    const timeoutFunction = () => {
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
    }
    setTimeout(timeoutFunction, 1000);
  }

  componentDidMount() {

    countWs = new WebSocket(`${URL_BASE}/getCaseCount`);
    countWs.onmessage = (e) => {
      caseCount = e.data;
    }

    this.waitFor(() => { return caseCount > 0 }, 5, (countRetrieved) => {
      if (!countRetrieved) {
        console.error('Failed to retrieve case count');
        TestComponent.markTestPassed(false);
        return;
      }

      if (caseCount < 1) {
        console.error(`Case count too low: [${caseCount}]`);
        TestModule.markTestPassed(false);
        return;
      } else {
        console.log(`Retrieved [${caseCount}] cases`);
      }

      //TestModule.markTestPassed(true);
      this.runCase(1);
    });
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
