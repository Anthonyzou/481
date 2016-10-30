const Api = require('scaleway'),
    client = new Api({token: process.env['SCALEWAY']});
const _ = require('lodash');

client.get('/servers', (err, res) => {
  _.map(res.body.servers, (a) => {
      console.log(`${a.private_ip} ${a.hostname}`)
  });
});
