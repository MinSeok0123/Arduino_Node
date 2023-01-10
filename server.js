const express = require("express");
const mysql = require("mysql2");
const cors = require("cors");

const app = express();

const port = process.env.PORT || 8000;

app.use(express.json());
app.use(cors());

// con.connect((err) => {
//   if (err) throw err;
//   console.log("Connected! With Stably");
// });

/** 아두이노로부터 값을 받아오는 API */
app.get("/test", (req, res) => {
  console.log(req.body);
  res.send("전송완료");
});

app.listen(port, () => {
  console.log("서버가 열렸습니다");
});
