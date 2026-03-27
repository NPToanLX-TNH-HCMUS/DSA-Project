const express = require('express');
const {execFile} = require('child_process');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 3000;

const ENGINE_PATH = path.join(__dirname, '../main.exe');

app.use(express.static(__dirname));

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, '../index.html'));
});

app.get('/search', (req, res) => {
  const word = req.query.word;
  if (!word) return res.status(400).json({error: 'No word provided'});

  const exePath = path.join(__dirname, 'main');

  execFile(exePath, [word], {cwd: __dirname}, (error, stdout, stderr) => {
    if (error) {
      console.error('Chi tiết lỗi C++:', stderr);
      return res.status(500).json({error: 'C++ Engine Error', details: stderr});
    }
    res.json({definition: stdout.trim()});
  });
});

app.get('/roadmap', (req, res) => {
  console.log('ROADMAP API HIT');
  const word = req.query.word;
  if (!word) return res.status(400).json({error: 'No word provided'});

  const exePath = path.join(__dirname, 'main');

  execFile(
      exePath, ['roadmap', word], {cwd: __dirname}, (error, stdout, stderr) => {
        if (error) {
          console.error('Roadmap error:', stderr);
          return res.status(500).json({error: 'C++ Roadmap Error'});
        }

        try {
          const data = JSON.parse(stdout);
          res.json(data);
        } catch (e) {
          res.status(500).json({error: 'Invalid JSON from C++'});
        }
      });
});

app.listen(PORT, '0.0.0.0', () => {
  console.log(`Server is running at: http://localhost:${PORT}`);
});