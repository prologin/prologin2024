// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright 2018 Sacha Delanoue
// Copyright 2019 Paul Guénézan
// Copyright 2014-2018 Godot Engine contributors

var dump_value = "the dump",
  players_value = {};

$("#replay").html(`
    <div id="container">
        <canvas id="canvas" oncontextmenu="event.preventDefault();" width="640" height="480">
            HTML5 canvas appears to be unsupported in the current browser.<br />
            Please try updating or use a different browser.
        </canvas>
    </div>
`);

$("body").append($("<script/>").attr("src", "/static/godot/prologin2024.js"));

$("#content").append(
  $("<div></div>").css("min-height", "300px").css("height", "300px"),
);

const content = document.getElementById("content");

const width = content.offsetWidth;
const height = (width * 1) / 1.66666666666;

const myWasm = "/static/godot/prologin2024";
const myPck = "/static/godot/prologin2024.pck";

const engine = new Engine();

const canvas = document.getElementById("canvas");
canvas.height = height;
canvas.width = width;

function print(text) {
  console.log(text);
}

function printError(text) {
  console.warn(text);
}

async function get_dump() {
  const url = $("#replay").data("match-dump-url");
  const r = await window.fetch(url);
  dump_value = await r.text();
}

Promise.all([
  // Load and init the engine
  engine.init(myWasm),
  // And the pck concurrently
  engine.preloadFile(myPck),

  get_dump(),
])
  .then(() => {
    // Now start the engine.
    return engine.startGame({
      mainPack: myPck,
      canvas: canvas,
      canvasResizePolicy: 0,
      onPrint: print,
      onPrintError: printError,
      executable: myWasm,
    });
  })
  .then(() => {
    console.log("Engine started!");
  });
