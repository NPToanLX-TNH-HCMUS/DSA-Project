const express = require("express");
const { spawn } = require("child_process");
const path = require("path");

const app = express();

app.use(express.json());
app.use(express.static(__dirname)); // phục vụ index.html

app.post("/search", (req, res) => {
    const input = req.body.input;

    if (!input) {
        return res.json({ result: "No input!" });
    }

    const process = spawn(path.join(__dirname, "Search.exe"));

    let output = "";
    let errorOutput = "";

    // gửi input vào C++
    process.stdin.write(input + "\n");
    process.stdin.end();

    process.stdout.on("data", (data) => {
        output += data.toString();
    });

    process.stderr.on("data", (data) => {
        errorOutput += data.toString();
    });

    process.on("close", (code) => {
        if (errorOutput) {
            res.json({ result: "ERROR:\n" + errorOutput });
        } else {
            res.json({ result: output });
        }
    });
});

app.listen(3000, () => {
    console.log("Server running at http://localhost:3000");
});