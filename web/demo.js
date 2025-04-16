Module.onRuntimeInitialized = () => { start(); }

const EMPTY = 0;
const WHITE = 1;
const BLACK = 2;
const UNCONSTRAINED = -1;
const FULL = 0;
const ORTHO = 1;
const FULL_EXCLUDE = 2;
const ORTHO_EXCLUDE = 3;

let col2str = [" ", "□", "■"];
let num2str = [
 ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"], // empty
 ["🄋", "➀", "➁", "➂", "➃", "➄", "➅", "➆", "➇", "➈"], // white
 ["⓿", "❶", "❷", "❸", "❹", "❺", "❻", "❼", "❽", "❾"], // black
];


function square2str(n, c) {
 if (n == UNCONSTRAINED)
 return col2str[c];
 else
 return num2str[c][n];
}


function setGridItem(i, j, color, ctx, cellWidth, cellHeight, value,status) {
 var cellX = i * cellWidth;
 var cellY = j * cellHeight;

 // Fill the square with the specified color
 ctx.fillStyle = color == 0 ? "gray" : color == 1 ? "white" : "black";
 ctx.fillRect(cellX, cellY, cellWidth, cellHeight);

 // Set the text color
 const textColor = color == 2 ? "white" : "black"; // Change text color as needed
 const definiteTextColor = status == 0 ?"red":status==1?textColor:"gray"
 // Display a number inside the cell
 ctx.font = "20px Arial";
 ctx.textAlign = "center";
 ctx.textBaseline = "middle";
 
 ctx.strokeStyle = definiteTextColor; // Change stroke color as needed
 ctx.lineWidth = 1; // Change stroke width as needed
 ctx.strokeText(value === -1 ? "" : value, cellX + cellWidth / 2, cellY + cellHeight / 2);
} 


function printGame(g,cellWidth,cellHeight,ctx) {
 
 var nb_rows = Module._nb_rows(g);
 var nb_cols = Module._nb_cols(g);
 for (var row = 0; row < nb_rows; row++) {
 for (var col = 0; col < nb_cols; col++) {
 var n = Module._get_constraint(g, row, col);
 var c = Module._get_color(g, row, col);
 var status = Module._get_status(g, row, col);
 setGridItem(row,col,c,ctx,cellWidth,cellHeight,n,status)
 }
 
 }

 
}

function generateGrid(g,rows,columns) {

 // Get the canvas element
 var canvas = document.getElementById("display");
 var ctx = canvas.getContext("2d");

 // Calculate cell size
 var cellWidth = canvas.width / columns;
 var cellHeight = canvas.height / rows;

 // Clear canvas
 ctx.clearRect(0, 0, canvas.width, canvas.height);

 // Draw the grid
 ctx.beginPath();
 for (var x = 0; x <= canvas.width; x += cellWidth) {
 ctx.moveTo(x, 0);
 ctx.lineTo(x, canvas.height);
 }

 for (var y = 0; y <= canvas.height; y += cellHeight) {
 ctx.moveTo(0, y);
 ctx.lineTo(canvas.width, y);
 }

 ctx.strokeStyle = "black";
 ctx.stroke();
 printGame(g,cellWidth,cellHeight,ctx)
 const grid={ctx,cellWidth,cellHeight}
 return grid
}

function start() {
 console.log("call start routine");
 // var g = Module._new_default();
 var g = Module._new_default();
 var canvas=document.getElementById("display")
 const grid=generateGrid(g,Module._nb_cols(g),Module._nb_cols(g))

 document.getElementById("undoBtn").addEventListener("click", function() {
    Module._undo(g);
    printGame(g, grid.cellWidth, grid.cellHeight, grid.ctx);
});

document.getElementById("redoBtn").addEventListener("click", function() {
    Module._redo(g);
    printGame(g, grid.cellWidth, grid.cellHeight, grid.ctx);
});

document.getElementById("restartBtn").addEventListener("click", function() {
    Module._restart(g);
    printGame(g, grid.cellWidth, grid.cellHeight, grid.ctx);
});
document.getElementById("solve").addEventListener("click", function() {
    Module._solve(g);
    printGame(g, grid.cellWidth, grid.cellHeight, grid.ctx);
    
});

document.getElementById("newGameBtn").addEventListener("click", function() {
    g = Module._new_random(6, 6, false, FULL, 0.6, 0.5); // Create a new random game
    printGame(g, grid.cellWidth, grid.cellHeight, grid.ctx);
});
 
 canvas.addEventListener("click", function(event) {
 // Get the coordinates of the click relative to the canvas
 var rect = canvas.getBoundingClientRect();
 var mouseX = event.clientX - rect.left;
 var mouseY = event.clientY - rect.top;
 
 // Calculate the grid coordinates of the clicked square
 var gridX = Math.floor(mouseX / grid.cellWidth);
 var gridY = Math.floor(mouseY / grid.cellHeight);
 
 let color=Module._get_color(g,gridX,gridY)
 let newColor=color==0?WHITE:color==1?BLACK:EMPTY
 let constraint=Module._get_constraint(g,gridX,gridY)
 Module._play_move(g,gridX,gridY,newColor)
 printGame(g,grid.cellWidth,grid.cellHeight,grid.ctx)
 
 });
 
}