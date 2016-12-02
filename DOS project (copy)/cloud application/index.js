//  index.js

//--------------------Declarations-----------------------------

// Declaring the modules required
const path = require('path')  
const express = require('express')  
const exphbs = require('express-handlebars')
const port = 3000
const app = express()


/*// udp connection
var dgram = require( "dgram" );
var client = dgram.createSocket( "udp4" );
var host = "192.168.0.17" ; 
var edge_ip = "192.168.0.14" ;
var edge_port = 8000;
client.bind( port, host );
console.log("udp_client connection established");*/


// tcp connection
var net = require('net');
var client_tcp = new net.Socket();
client_tcp.connect(3333, '192.168.0.14', function() {
  console.log('tcp_server connection established');

});

//client_tcp.write('Hello');

                                                                                                                       
// Listen to a port
app.listen(port, (err) => {  
  if (err) {
    return console.log('something bad happened', err)
  }

  console.log(`server is listening on ${port}`)
}) 

app.use("/", express.static(__dirname + '/'));

// ---------------------HTTP Requests-----------------------------

// GET requests
app.get('/', (request, response) => {  
  console.log(resquest)
  response.sendFile(__dirname + '/index.html')
});


// POST requests

// BBB 1 containing the LEDs

// red button 
app.post('/redbtn', function (req, res) {
  var message = new Buffer("LED_r_on" );
	client_tcp.write(message, function(msg, rinfo) {
   console.log( "sent message to the edge" );});
  res.redirect('/');
});

// green button
app.post('/greenbtn', function (req, res) {
  var message = new Buffer("LED_g_on" );
	client_tcp.write(message, function(msg, rinfo) {
   console.log( "sent message to the edge" );});
  res.redirect('/');
});

// blue button
app.post('/bluebtn', function (req, res) {
  var message = new Buffer("LED_b_on");
	client_tcp.write(message, function(msg, rinfo) {
   console.log( "sent message to the edge" );});
  res.redirect('/');
});

// led 1  red on
app.post('/led1_on', function (req, res) {
  var message = new Buffer("LED_r_on" );
  client_tcp.write(message, function(msg, rinfo) {
   console.log( "sent message to the edge" );});
  res.redirect('/');
});

// led 1  red off
app.post('/led1_off', function (req, res) {
  var message = new Buffer("LED_r_off" );
  client_tcp.write(message, function(msg, rinfo) {
   console.log( "sent message to the edge" );});
  res.redirect('/');
});

// led 2  green on
app.post('/led2_on', function (req, res) {
  var message = new Buffer("LED_g_on" );
  client_tcp.write(message, function(msg, rinfo) {
   console.log( "sent message to the edge" );});
  res.redirect('/');
});

// led 2  green off
app.post('/led2_off', function (req, res) {
  var message = new Buffer("LED_g_on" );
  client_tcp.write(message, function(msg, rinfo) {
   console.log( "sent message to the edge" );});
  res.redirect('/');
});

// led 3  blue on
app.post('/led3_on', function (req, res) {
  var message = new Buffer("LED_b_on" );
  client_tcp.write(message, function(msg, rinfo) {
   console.log( "sent message to the edge" );});
  res.redirect('/');
});

// led 3 blue off 
app.post('/led3_off', function (req, res) {
  var message = new Buffer("LED_b_off" );
  client_tcp.write(message, function(msg, rinfo) {
   console.log( "sent message to the edge" );});
  res.redirect('/');
});

// BBB 2 containing the temperature sensor and buzzer
app.post('/tempbtn', function (req, res) {
var message = new Buffer("readtemp" );
  client_tcp.write(message, function(msg, rinfo) {
   console.log( "sent message to the edge" );});
  res.redirect('/tempbtn');
});

// buzzer on
app.post('/buzz_on', function (req, res) {
  var message = new Buffer("buzz_on" );
  client_tcp.write(message, function(msg, rinfo) {
   console.log( "sent message to the edge" );});
  res.redirect('/');
});

// buzzer off
app.post('/buzz_off', function (req, res) {
  var message = new Buffer("buzz_off" );
  client_tcp.write(message, function(msg, rinfo) {
   console.log( "sent message to the edge" );});
  res.redirect('/');
});


// response from BBB tcp
client_tcp.on('data', function(data) {
  console.log('Received: ' + data);
});


// Export variables
module.exports.app = app;
module.exports.exphbs = exphbs;
module.exports.express = express;
module.exports.path = path;