import {request, _ui} from './lib/bundle.js';

// Colorpicker Addon Initializer
$(function () {
  $('#cp-btn').colorpicker({
    popover: {
      title: 'Adjust the color',
      placement: 'top'
    }
  });
});

const hexToRgb = function hexToRgb(str) {
  return str.match(/.{1,2}/g).map(pair => parseInt(pair, 16));
};

_ui.loaded(() => {
  _ui.defLoadState({
    element: ".alert",
    default: ["alert", "alert-dark"],
    loading: ["alert", "alert-warning"],
    success: ["alert", "alert-success"],
    error: ["alert", "alert-danger"],
  });

  const updateBtnColor = function updateBtnColor() {
    let btn = _ui.get({id: "cp-btn"});

    _ui.get({id: "request"}).disabled = false;
    _ui.changeStyle(btn, {backgroundColor: btn.value});
  }

  _ui.focusout(updateBtnColor, {id: "cp-btn"});

  const sendRequest = function sendRequest() {
    let hexVal = _ui.get({id: 'cp-btn', props: ["value"]}).value.substring(1);
    let rgbVals = hexToRgb(hexVal);
    request.rgb(...rgbVals);
  };

  _ui.click(sendRequest, {id: "request"});

  _ui.click(request.test, {id: "testreq"});
});