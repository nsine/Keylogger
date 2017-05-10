const net = require('net');
const express = require('express');
const http = require('http');
const app = express();

const encoding = 'utf-16le';
const portForSlaves = 5000;
const portForMaster = 3000;

let slaves = [];

// Routes for master

app.get('/', (req, res) => {
    let {id, command} = req.query;

    let slave = slaves.find(s => s.id == id);
    if (!slave) {
        res.status(404).send('No slaves with this id');
        return;
    }

    if (!slave.online) {
        res.status(400).send('Slave is offline');
        return;
    }

    slave.socket.write(wrapToBuffer(command));
    slave.socket.on('data', data => {
        res.send(data.toString(encoding));
        slave.socket.removeAllListeners('data');
    });
});

app.get('/connect', (req, res) => {
    let id = req.query.id;

    if (!id) {
        res.status(400).send('Id is required');
        return;
    }

    let slave = slaves.find(s => s.id == id);
    if (!slave) {
        res.status(404).send('Slave with this id not found');
        return;
    }

    if (!slave.online) {
        res.status(400).send('Slave is offline');
        return;
    }

    res.send(slave.computerName);
});

app.get('/list', (req, res) => {
    let response = slaves.map(s => {
        return {
            id: s.id,
            name: s.computerName,
            online: s.online
        }
    });

    res.send(JSON.stringify(response));
});

const server = http.createServer(app);
server.listen(portForMaster, () => {
    console.log('connector started');
});

// Socket server for slaves

net.createServer(socket => {
    let address = socket.remoteAddress + ":" + socket.remotePort;

    socket.on('data', data => {
        let req = data.toString();
        let [id, computerName] = req.split(' ');

        let slave = slaves.find(s => s.id == id);

        if (!slave) {
            console.log(`new slave ${address} connected`);
            slaves.push({
                address: address,
                computerName: computerName,
                online: true,
                socket: socket,
                id: id
            });
        } else {
            console.log(`slave online ${id} ${computerName}`)
            slave.computerName = computerName;
            slave.online = true;
            slave.socket = socket;
            slave.address = address;
        }

        socket.write(wrapToBuffer('ok'));
        socket.removeAllListeners('data');
    });

    socket.on('error', onSlaveDisconnected);
    socket.on('end', onSlaveDisconnected);

    function onSlaveDisconnected() {
        console.log(`${address} disconnected`);
        let slave = slaves.find(s => s.address == address);
        slave.online = false;
    }
}).listen(portForSlaves);


function wrapToBuffer(str) {
    return Buffer.from(str, encoding);
}