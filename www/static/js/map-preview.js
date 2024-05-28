$("#map-preview").html(`
    <div id="container">
        <canvas id="canvas" oncontextmenu="event.preventDefault();" width="640" height="480">
            HTML5 canvas appears to be unsupported in the current browser.<br />
            Please try updating or use a different browser.
        </canvas>
    </div>
`);

$("body").append($("<script/>").attr("src", "/static/godot/prologin2024.js"));

const width = content.offsetWidth;
const height = (width * 1) / 1.66666666666;

const myWasm = "/static/godot/prologin2024";
const myPck = "/static/godot/prologin2024.pck";

const map_content = $("#map-contents").html();
console.log(map_content);

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

Promise.all([
  // Load and init the engine
  engine.init(myWasm),
  // And the pck concurrently
  engine.preloadFile(myPck),
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
