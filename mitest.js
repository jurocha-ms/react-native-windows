// See https://github.com/crossbario/autobahn-testsuite/blob/v0.8.2/autobahntestsuite/autobahntestsuite/web/fuzzingserver/test_browser.html

const WebSocket = require('ws');

var statusWs = null;
var testWs = null;
var wsuri = 'ws://localhost:9001';
var caseCount = 1;
var agent = "unknown";

function updateStatus(msg) {
    console.log(msg);
}

function openWebSocket(ws_uri)
{
    return new WebSocket(ws_uri);
}

function updateReports() {
    var ws_uri = wsuri + "/updateReports?agent=unknown";

    statusWs = openWebSocket(ws_uri);

    statusWs.onopen =
       function(e)
       {
          updateStatus("Updating reports ..");
       }

    statusWs.onclose =
       function(e)
       {
          statusWs = null;
          updateStatus("Reports updated.");
          updateStatus("Test suite finished!");
       }
}

function runNextCase(currentCaseId)
{
   var ws_uri = wsuri + "/runCase?case=" + currentCaseId + "&agent=" + agent;

   testWs = openWebSocket(ws_uri);
   testWs.binaryType = "arraybuffer";

   testWs.onopen =
      function(e)
      {
         updateStatus("Executing test case " + currentCaseId + "/" + caseCount);
      }

   testWs.onclose =
      function(e)
      {
         testWs = null;

         currentCaseId = currentCaseId + 1;
         if (currentCaseId <= caseCount)
         {
            runNextCase();
         }
         else
         {
            updateStatus("All test cases executed.");
            updateReports();
         }
      }

   testWs.onerror = (e) => {
        console.error(`[FAIL] ${e}`)
   }

   testWs.onmessage =
      function(e)
      {
         testWs.send(e.data);
      }
}

runNextCase(process.argv[2]);

// var ws = new WebSocket('ws://localhost:9001/runCase?case=1&agent=unknown');ws.binaryType = "arraybuffer";ws.onopen = (e) => { console.log("Executing test case"); };ws.conclose = (e) => { ws = null; console.log("All executed"); };ws.onmessage = (e) => { console.log(`Got data: ${e.data}`); ws.send(e.data); }; ws.onerror = (e) => { console.log('[FAIL]'); }
