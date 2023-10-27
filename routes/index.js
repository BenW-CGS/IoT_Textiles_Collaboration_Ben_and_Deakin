//Setting variables for other modules.
const express= require('express')
const router = express.Router()
const {fetch} = require('undici')


router.get('/', async(req, res) => {
    try {
        //data is from esp32
        const data = await getApiData()
        //The response to webpage is the index page with the data accessible.
        res.render('index', {obj: data} )
    } catch (error) {
        //more error info.
        console.error('Error:', error);
        res.status(500).send('An error occurred while fetching data.');
    }
})



async function getApiData() {
    try { 
        //set variable to get data from ESP32
        const res = await fetch('http://192.168.1.141')

        if (!res.ok) {
            //If I can't get any response, show error
            throw new Error('Failed to fetch data')
        }
        //Variable for data from ESP32 (name, temp, humidity, etc.)
        const txt = await res.json()

        const data = {
            sensName : txt.name,
            location : txt.variables.location,
            id: txt.id,
            hardware: txt.hardware,
            temperature : txt.variables.temperature,
            humidity : txt.variables.humidity,
            fireindex : txt.variables.fireindex,
        }
        //Show data here.
        console.log(txt)
        //Add data to website.
        return data;
//If error, show it.
    } catch (error) {
        console.error('Error:', error)
        throw error
    }
    
}

module.exports = router
