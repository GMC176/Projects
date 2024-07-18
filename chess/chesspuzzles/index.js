const express = require('express');
const bodyParser = require('body-parser');
const mysql = require('mysql2');
const bcrypt = require('bcryptjs');
const session = require('express-session');
const path = require('path');
const { time } = require('console');
const app = express();

const db = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: 'gaurav123',
    database: 'chesschat',
});

app.use(session({
    secret: 'chess_with_gaurav',
    resave: true,
    saveUninitialized: true,
    cookie: {

        // Session expires after 1 min of inactivity.
        expires: 6000000
    }
}));

// Connect to MySQL
db.connect((err) => {
    if (err) {
        console.error('Error connecting to MySQL:', err);
        throw err;
    }
    console.log('MySQL Connected...');
});

app.use(express.static('public'));
app.use(express.static('views'));
app.set('view engine', 'ejs')

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

app.use('/static',express.static(path.join(__dirname,"public")));
app.use(session({
    secret: 'your_secret_key',
    resave: false,
    saveUninitialized: true,
    cookie: { secure: false ,maxAge: 24 * 60 * 60 * 1000}
}));



app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'views', 'index.html'));
});

app.get('/views/wel', (req, res) => {
    console.log(req.session.user);
    res.sendFile(path.join(__dirname, 'views', 'wel.html'));
});

app.get('/views/stock.html', (req, res) => {
    console.log(req.session.user);
    res.sendFile(path.join(__dirname, 'views', 'stock.html'));
});

app.get('/views/img/chesspieces/wikipedia/:i', (req, res) => {
    const imageName = req.params.i;
    const imagePath = path.join(__dirname, 'views', 'img', 'chesspieces', 'wikipedia', imageName);
    res.sendFile(imagePath);
});


app.get('/views/puzzle.ejs', (req, res) => {
    db.query('SELECT * FROM puzzles order by upvotes desc', (err, rows, fields) => {
        if (err) {
            console.error('Error fetching puzzles:', err);
            return res.status(500).json({ success: false, message: 'Failed to fetch puzzles' });
        }
        console.log(rows);
        res.render('puzzle.ejs', { title: 'Puzzles', items: rows });
    });
});



app.post('/login',(req,res)=>{
    const { loginId, password } = req.body;
    let sql = 'SELECT * FROM users WHERE loginId = ?';
    db.query(sql, [loginId], (err, results) => {
        if (err || results.length === 0) {
            console.error('Error logging in:', err);
            return res.json({ success: false, message: 'User not found' });
        }

        const user = results[0];
        if (bcrypt.compareSync(password, user.password)) {
            req.session.loggedIn = true;
            req.session.user = user;
            req.session.save();
            res.redirect(`views/wel`)
            
        } else {
            res.json({ success: false, message: 'Incorrect password' });
        }
    });
})

app.get('/session-user', (req, res) => {
    res.json({ user: req.session.user.loginId });
});

app.post('/createpuzzle', (req, res) => {
    const { fen, solution,num,playby } = req.body;
    const user = req.session.user;

    if (!req.session.user) {
        return res.status(401).json({ success: false, message: 'User not logged in' });
    }
    // Example SQL query to insert puzzle into database
    const sql = 'INSERT INTO puzzles (fen, solution, userId,nummoves,whitetoplay) VALUES (?, ?, ?,?,?)';
    db.query(sql, [fen, solution, user.id,num,playby], (err, result) => {
        if (err) {
            console.error('Error creating puzzle:', err);
            res.status(500).json({ success: false, message: 'Failed to create puzzle' });
        } else {
            console.log('Puzzle created successfully');
            res.json({ success: true, message: 'Puzzle created successfully' });
        }
    });
});


app.post('/upvote/:id', (req, res) => {
    const id = req.params.id;
    const user = req.session.user;

    if (!user) {
        console.log('User not logged in');
        return res.status(401).json({ success: false, message: 'User not logged in' });
    }

    const sql1 = 'SELECT * FROM user_upvotes WHERE user_id = ? AND puzzle_id = ?';
    db.query(sql1, [user.id, id], (err, result) => {
        if (err) {
            return res.status(500).send('Error checking upvote status');
        }

        if (result.length !== 0) {
            return res.status(400).json({ success: false, message: 'Already upvoted this puzzle' });
        } else {
            const sql2 = 'UPDATE puzzles SET upvotes = upvotes + 1 WHERE id = ?';
            db.query(sql2, [id], (err) => {
                if (err) {
                    return res.status(500).send('Error updating puzzles while upvoting');
                }

                const sql3 = 'INSERT INTO user_upvotes (user_id, puzzle_id) VALUES (?, ?)';
                db.query(sql3, [user.id, id], (err) => {
                    if (err) {
                        return res.status(500).send('Error updating user_upvotes while upvoting');
                    }

                    res.json({ success: true, message: 'Upvoted!!!' });
                });
            });
        }
    });
});

app.post('/upvotecomment/:id', (req, res) => {
    const id = req.params.id;
    const user = req.session.user;

    if (!user) {
        console.log('User not logged in');
        return res.status(401).json({ success: false, message: 'User not logged in' });
    }

    const sql1 = 'SELECT * FROM user_upvotescomments WHERE user_id = ? AND comment_id = ?';
    db.query(sql1, [user.id, id], (err, result) => {
        if (err) {
            return res.status(500).send('Error checking upvote status');
        }

        if (result.length !== 0) {
            return res.status(400).json({ success: false, message: 'Already upvoted this comment' });
        } else {
            const sql2 = 'UPDATE comments SET upvotes = upvotes + 1 WHERE id = ?';
            db.query(sql2, [id], (err) => {
                if (err) {
                    return res.status(500).send('Error updating puzzles while upvoting');
                }

                const sql3 = 'INSERT INTO user_upvotescomments (user_id, comment_id) VALUES (?, ?)';
                db.query(sql3, [user.id, id], (err) => {
                    if (err) {
                        return res.status(500).send('Error updating user_upvotes while upvoting');
                    }

                    res.json({ success: true, message: 'Upvoted!!!' });
                });
            });
        }
    });
});

app.post('/scoreup/:id', (req, res) => {
    const puzzleId = req.params.id;
    const user = req.session.user;

    if (!user) {
        console.log('User not logged in');
        return res.status(401).json({ success: false, message: 'User not logged in' });
    }

    let sql = "SELECT * FROM answered WHERE puzzle_id = ? AND id = ?";
    db.query(sql, [puzzleId, user.id], (err, result) => {
        if (err) {
            console.error('Error checking answered status:', err);
            return res.status(500).json({ success: false, message: 'Error checking answered status' });
        }

        if (result.length !== 0) {
            return res.status(403).json({ success: false, message: 'Already answered it before' });
        }

        let sql2 = "UPDATE users SET score = score + 1 WHERE id = ?";
        db.query(sql2, [user.id], (err, results) => {
            if (err) {
                console.error('Error updating score:', err);
                return res.status(500).json({ success: false, message: 'Error updating score' });
            }

            let sql3 = "INSERT INTO answered (puzzle_id, id) VALUES (?, ?)";
            db.query(sql3, [puzzleId, user.id], (err, resultss) => {
                if (err) {
                    console.error('Error inserting into answered table:', err);
                    return res.status(500).json({ success: false, message: 'Error inserting into answered table' });
                }

                res.json({ success: true, message: 'Score increased by 1' });
            });
        });
    });
});


app.get('/discussion/:id', (req, res) => {
    const puzzleid = req.params.id;
    const puzzlefen=req.params.fen
    const sql = "SELECT * FROM comments WHERE puzzle_id = ?";
    const user=req.session.user;
    if(!user){
        console.log('User not logged in');
        return res.status(401).json({ success: false, message: 'User not logged in' });
    }
    db.query(sql, [puzzleid], (err, rows) => {
        if (err) {
            console.error('Error fetching comments:', err);
            return res.status(500).send('Error fetching comments');
        }
        let sql1="select * from puzzles where id=?";
        db.query(sql1,[puzzleid],(err,puzzleRows)=>{
            const puzzlefen = puzzleRows.length > 0 ? puzzleRows[0].fen : '';
        res.render('discussion', { title: 'Puzzles', items: rows, puzzle_id: puzzleid,puzzle_fen:puzzlefen });
        });
    });
});

app.get('/discussion/:sortby/:id', (req, res) => {
    const puzzleid = req.params.id;
    const sorttech = req.params.sortby;
    const user = req.session.user;
	console.log(puzzleid);
    if (!user) {
        console.log('User not logged in');
        return res.status(401).json({ success: false, message: 'User not logged in' });
    }

    if (sorttech === "recent") {
        let sql = "SELECT * FROM comments WHERE puzzle_id = ?";
        db.query(sql, [puzzleid], (err, rows) => {
            if (err) {
                console.error('Error fetching comments:', err);
                return res.status(500).send('Error fetching comments');
            }
            let sql1 = "SELECT * FROM puzzles WHERE id = ?";
            db.query(sql1, [puzzleid], (err, puzzleRows) => {
                if (err) {
                    console.error('Error fetching puzzle:', err);
                    return res.status(500).send('Error fetching puzzle');
                }
                const puzzlefen = puzzleRows.length > 0 ? puzzleRows[0].fen : '';
                res.render('discussion', { title: 'Puzzles', items: rows, puzzle_id: puzzleid, puzzle_fen: puzzlefen });
            });
        });
    } else if (sorttech === "popular") {
        let sql = "SELECT * FROM comments WHERE puzzle_id = ? ORDER BY upvotes DESC";
        db.query(sql, [puzzleid], (err, rows) => {
            if (err) {
                console.error('Error fetching comments:', err);
                return res.status(500).send('Error fetching comments');
            }
            let sql1 = "SELECT * FROM puzzles WHERE id = ?";
            db.query(sql1, [puzzleid], (err, puzzleRows) => {
                if (err) {
                    console.error('Error fetching puzzle:', err);
                    return res.status(500).send('Error fetching puzzle');
                }
                const puzzlefen = puzzleRows.length > 0 ? puzzleRows[0].fen : '';
                res.render('discussion', { title: 'Puzzles', items: rows, puzzle_id: puzzleid, puzzle_fen: puzzlefen });
            });
        });
    } else {
        let sql2 = "SELECT * FROM puzzles WHERE id = ?";
        db.query(sql2, [puzzleid], (err, puzzleRows) => {
            if (err) {
                console.error('Error fetching puzzle:', err);
                return res.status(500).send('Error fetching puzzle');
            }

            const uid = puzzleRows.length > 0 ? puzzleRows[0].userId : null;
            if (!uid) {
                return res.status(404).send('Puzzle not found');
            }

            let sql3 = "SELECT * FROM users WHERE id = ?";
            db.query(sql3, [uid], (err, userRows) => {
                if (err) {
                    console.error('Error fetching user:', err);
                    return res.status(500).send('Error fetching user');
                }

                const name = userRows.length > 0 ? userRows[0].loginId : null;
                if (!name) {
                    return res.status(404).send('User not found');
                }

                let sql4 = "SELECT * FROM comments WHERE userId = ?";
                db.query(sql4, [name], (err, commentRows) => {
                    if (err) {
                        console.error('Error fetching comments:', err);
                        return res.status(500).send('Error fetching comments');
                    }
                    let sql1 = "SELECT * FROM puzzles WHERE id = ?";
                    db.query(sql1, [puzzleid], (err, puzzleRows) => {
                        if (err) {
                            console.error('Error fetching puzzle:', err);
                            return res.status(500).send('Error fetching puzzle');
                        }
                        const puzzlefen = puzzleRows.length > 0 ? puzzleRows[0].fen : '';
                        console.log("done");
                        res.render('discussion', { title: 'Puzzles', items: commentRows, puzzle_id: puzzleid, puzzle_fen: puzzlefen });
                    });
                });
            });
        });
    }
});


app.post('/comments/:id', (req, res) => {
    const puzzleId = req.params.id;
    const user = req.session.user;
    console.log("dagk");
    if (!user) {
        console.log('User not logged in');
        return res.status(401).json({ success: false, message: 'User not logged in' });
    }

    const { message } = req.body; // Destructuring to get message
    if (!message) {
        return res.status(400).json({ success: false, message: 'Message is required' });
    }
    const currentTime = new Date(); 
    const sql = "INSERT INTO comments (puzzle_id, userId, message) VALUES (?, ?, ?)";
    console.log(`Inserting comment: ${message} for puzzleId: ${puzzleId} by user: ${user.id}`);

    db.query(sql, [puzzleId, user.loginId, message], (err, result) => {
        if (err) {
            console.error('Error inserting comment:', err);
            return res.status(500).json({ success: false, message: 'Error inserting comment' });
        }
        res.json({ success: true, message: 'Comment posted',username:user.loginId,created_at:currentTime  });
    });
});




app.post('/signup', (req, res) => {
    const { loginId, password } = req.body;
    const hashedPassword = bcrypt.hashSync(password, 10);

    let sql = 'INSERT INTO users (loginId, password) VALUES (?, ?)';
    db.query(sql, [loginId, hashedPassword], (err, result) => {
        if (err) {
            console.error('Error signing up:', err);
            return res.json({ success: false, message: 'Failed to register user' });
        }
        res.redirect('/');
    });
});

app.listen(3000, () => {
    console.log('Server started on port 3000');
});

