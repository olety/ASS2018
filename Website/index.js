const express = require('express');
const path = require('path');
var fs = require('fs');

const app = express();
app.use(express.json()); // to support JSON-encoded bodies
app.use(express.urlencoded()); // to support URL-encoded bodies
app.set('view engine', 'pug');
app.set('views', path.join(__dirname, '/Views'));

const file = path.join(__dirname, '/scores.json');

var scores = JSON.parse(fs.readFileSync(file, 'utf8')) || [];

app.get('/', (req, res) => {
    console.log(scores);
    res.render("index", { data: scores });
});
app.post('/', (req, res) => {
    const obj = { name: (req.body.name || ""), score: (req.body.score || 0) };
    if (obj.name && obj.score) {
        scores.push({ name: req.body.name, score: req.body.score })
        scores.sort(function(a, b) {
            return b.score - a.score;
        });
        fs.writeFileSync(file, JSON.stringify(scores), 'utf-8');
        res.send('OK')
    } else {
        res.status(400)
        res.send("Bad Request")
    }
});
app.listen(5656, () => {
    console.log('http://localhost:5656')
});