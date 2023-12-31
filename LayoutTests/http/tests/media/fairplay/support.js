function uInt8ArrayToString(array) {
    var uint8array = new Uint8Array(array.buffer);
    return String.fromCharCode.apply(null, uint8array);
}

function uInt16ArrayToString(array) {
    var uint16array = new Uint16Array(array.buffer);
    return String.fromCharCode.apply(null, uint16array);
}

function base64DecodeUint8Array(input) {
    var raw = window.atob(input);
    var rawLength = raw.length;
    var array = new Uint8Array(new ArrayBuffer(rawLength));

    for(i = 0; i < rawLength; i++)
        array[i] = raw.charCodeAt(i);

    return array;
}

function stringToUInt8Array(str)
{
   var array = new Uint8Array(str.length);
   for (var i=0; i<str.length; i++)
        array[i] = str.charCodeAt(i);
   return array;
}

function stringToUint16Array(string) {
    var buffer = new ArrayBuffer(string.length*2); // 2 bytes for each char
    var array = new Uint16Array(buffer);
    for (var i=0, strLen=string.length; i<strLen; i++) {
        array[i] = string.charCodeAt(i);
    }
    return array;
}

function base64EncodeUint8Array(input) {
    var keyStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    var output = "";
    var chr1, chr2, chr3, enc1, enc2, enc3, enc4;
    var i = 0;

    while (i < input.length) {
        chr1 = input[i++];
        chr2 = i < input.length ? input[i++] : Number.NaN; // Not sure if the index
        chr3 = i < input.length ? input[i++] : Number.NaN; // checks are needed here

        enc1 = chr1 >> 2;
        enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
        enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
        enc4 = chr3 & 63;

        if (isNaN(chr2)) {
            enc3 = enc4 = 64;
        } else if (isNaN(chr3)) {
            enc4 = 64;
        }
        output += keyStr.charAt(enc1) + keyStr.charAt(enc2) +
            keyStr.charAt(enc3) + keyStr.charAt(enc4);
    }
    return output;
}

function failTestWithException(error) {
    if (error instanceof Error) {
        consoleWrite(error.toString());
        error.stack.split(/\r?\n/).forEach(line => { consoleWrite(line); });
        failTest();
    }
    failTest(error);
}