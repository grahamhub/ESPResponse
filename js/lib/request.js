import {minui as _ui} from './minui/minui.js';

export let request;

(function() {
  const Request = (function() {
    const TIMEOUT = 15000;
    const URL = 'http://192.168.1.81:8080';

    let xhRequest = new XMLHttpRequest();

    xhRequest.addEventListener('loadstart', () => {
      _ui.get({query: '.alert'})[0].innerText = "Awaiting response...";
      _ui.startLoad('.alert');
    });

    xhRequest.addEventListener('loadend', (event) => {
      _ui.get({query: '.alert'})[0].innerText = event.target.response.message;
      switch (event.target.status) {
        case 200:
        case 202:
          _ui.stopLoad('.alert', 'success');
          break;
        case 400:
        case 403:
          _ui.stopLoad('.alert', 'error');
          break;
        default:
          _ui.stopLoad('.alert', 'error');
          break;
      }
    });

    const testRequest = async function testRequest() {
      xhRequest.open('GET', `${URL}/test`);
      xhRequest.timeout = TIMEOUT;
      xhRequest.responseType = 'json';

      xhRequest.send();
    };

    const ledRequest = function ledRequest(r, g, b) {
      xhRequest.open('GET', `${URL}/leds?r=${r}&g=${g}&b=${b}`);
      xhRequest.timeout = TIMEOUT;
      xhRequest.responseType = 'json';

      xhRequest.send();
    }

    return {

      test() {
        testRequest();
      },

      rgb(r, g, b) {
        ledRequest(r, g, b);
      },

      init() {
        return this;
      }
    }
  })();

  request = Object.create(Request).init();
})();