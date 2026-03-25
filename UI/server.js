const express = require('express');
const {execFile} = require('child_process');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 3000;  // Render sẽ dùng PORT này

// Đường dẫn đến file engine.exe
const ENGINE_PATH = path.join(__dirname, '../main.exe');

// --- THÊM DÒNG NÀY ---
app.use(express.static(__dirname));  // Cho phép truy cập index.html, style.css

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, '../index.html'));
});
// ----------------------

app.get('/search', (req, res) => {
  const word = req.query.word;
  if (!word) return res.status(400).json({error: 'No word provided'});

  // Đường dẫn tuyệt đối đến file main.exe
  const exePath = path.join(__dirname, 'main');

  // execFile nhận tham số word truyền vào mảng []
  // Cách này bỏ qua Shell nên không bao giờ lo lỗi "not recognized"
  execFile(exePath, [word], {cwd: __dirname}, (error, stdout, stderr) => {
    if (error) {
      console.error('Chi tiết lỗi C++:', stderr);  // In ra lỗi thật của C++
      return res.status(500).json({error: 'C++ Engine Error', details: stderr});
    }
    res.json({definition: stdout.trim()});
  });
});

// Chạy server
app.listen(PORT, () => {
  console.log(`Server đang chạy tại: http://localhost:${PORT}`);
  console.log(`Thử tra cứu: http://localhost:${PORT}/search?word=trie`);
});