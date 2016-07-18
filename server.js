var express = require('express');
var server = express();
var bodyParser=require('body-parser');
var cors  = require('cors');
var sqlite3 = require('sqlite3');

server.use('/', express.static(__dirname+'/public'));
server.use(bodyParser.json());
server.use(cors());
server.use(bodyParser.urlencoded({extended:true}));

var db = new sqlite3.Database('./db.s3db');

	server.post('/receiver', function(req,res){
		var token = req.body.token;
		var value = req.body.value;

		db.get("SELECT * FROM usuarios WHERE token = ?",[token], function(err,result){
			if(result){
				db.run("INSERT INTO data (uuid, value, fecha) VALUES (?,?,?)",[result.uuid, value, new Date()], function(error, result){
					console.log("registro almacenado: " + value);
					res.status(200).send("registro almacenado: " + value);
				});
			}else{
				console.log("token invalido")
				return res.status(401).send({ message: 'Token Invalido' });
			}
		});
	});

	server.listen(3000);
