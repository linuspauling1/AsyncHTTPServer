#include <avr/pgmspace.h>

const char indexHtml[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>GPIO Control</title>
    <style>
        body {
            font-family: "times new roman", sans-serif;
            zoom: 1.1;
            background-color: antiquewhite;
            text-align: center;
        }
        .switch {
            position: relative;
            display: inline-block;
            width: 60px;
            height: 30px;
            top: 5px;
            left: 5px;
        }
        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            bottom: 0;
            right: 0;
            background-color: #ccc;
            border-radius: 24px;
            transition: .4s;
        }
        .slider::after {
            position: absolute;
            content: "";
            top: 3px;
            left: 3px;
            width: 24px;
            height: 24px;
            background-color: white;
            border-radius: 12px;
            transition: .4s;
        }
        .switch input {
            display: none;
        }
        .switch input:checked + .slider {
            background-color: green;
        }
        .switch input:checked + .slider::after {
            transform: translateX(30px);
        }
        .gpio {
            position: relative;
            display: inline-block;
            zoom: 1.5;
        }
    </style>
</head>
<body>
    <h1>Select GPIO</h1>
    <hr><br>
    <div class="gpio">Built-in LED:</div>
    <label class="switch">
        <input type="checkbox" id="builtin" %PLACEHOLDER_BUILTIN%>
        <span class="slider"></span>
    </label><br>
</body>
</html>
<script>
    const builtin = document.getElementById("builtin")
    builtin.addEventListener("change",() => {
        if(builtin.checked) {
            fetch('/on')
            .then(response => {
                if(!response.ok)
                    throw new Error('HTTP Error')
                return response.text()
            })
            .then(textData => {
                alert(textData)
            })
            .catch(error => alert(error))
        }
        else {
            fetch('/off')
            .then(response => {
                if(!response.ok)
                    throw new Error('HTTP Error')
                return response.text()
            })
            .then(textData => {
                alert(textData)
            })
            .catch(error => alert(error))
        }
    });
</script>
)=====";