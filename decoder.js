function Decoder(bytes, port) {
    var result = "";
    for (var i = 0; i < 5; i++) {
      result += String.fromCharCode(parseFloat(bytes[i]));
    }
   
    var result2 = "";
    for (var j = 5; j < 10; j++) {
      result2 += String.fromCharCode(parseFloat(bytes[j]));
    }
    
    
    return { temp: result, hum: result2 };
   }