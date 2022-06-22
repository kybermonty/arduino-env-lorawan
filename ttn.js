function Decoder(bytes) {
    var temperature = ((bytes[0] << 8) | bytes[1]) / 10.0;
    var humidity = bytes[2] / 2;
    var pressure = ((bytes[3] << 8) | bytes[4]) * 2.0;
    var illuminance = ((bytes[5] << 8) | bytes[6]);

    return {
        temperature: temperature,
        humidity: humidity,
        pressure: pressure,
        illuminance: illuminance
    };
}

function decodeUplink(input) {
    return {
        data: Decoder(input.bytes),
        warnings: [],
        errors: []
    };
}
