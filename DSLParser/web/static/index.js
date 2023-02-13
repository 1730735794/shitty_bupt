window.onload = function init() {
    document.title = "test"
    let date = new Date();
    let timeStamp = document.createElement('div');
    timeStamp.className = 'item item-center';
    timeStamp.innerHTML = `<span>${date.toLocaleString()}</span>`;
    let firstChild = document.querySelector('.content').firstChild;
    if (firstChild) {
        document.querySelector('.content').insertBefore(timeStamp, firstChild);
    }
    else {
        document.querySelector('.content').appendChild(timeStamp);
    }
    document.querySelector('#textarea').value = '';
    $.ajax('http://127.0.0.1:8000/hi', {
            type: 'POST',
            success:function (result) {
                robotSend(result)
                console.log(result)
            },
            error:function (err) {
                robotSend("Err" + err)
                console.log(err);
            }
        })
}
function createNewBubble(text, flag)
{
    let item = document.createElement('div');
    if(flag){
        item.className = 'item item-right';
        item.innerHTML = `<div class="bubble bubble-left">${text}</div><div class="avatar"><img src="static/me.jpg" /></div>`;
    }
    else{
        item.className = 'item item-left';
        item.innerHTML = `<div class="avatar"><img src="static/robot.jpg" /></div><div class="bubble bubble-left">${text}</div>`;
    }
    
    document.querySelector('.content').appendChild(item);
}
function userSend() {
    let text = document.querySelector('#textarea').value;
    if (!text) {
        alert('请输入内容');
        return;
    }
    createNewBubble(text, 1);
    document.querySelector('#textarea').value = '';
    $.ajax('http://127.0.0.1:8000/reply', {
            type: 'POST',
            data: JSON.stringify({ msg: text }),
            contentType: 'application/json;charset:utf-8;',
            success:function (result) {
                robotSend(result)
                console.log(result)
            },
            error:function (err) {
                robotSend("Err" + err)
                console.log(err);
            }
        })
    //滚动条置底
    let height = document.querySelector('.content').scrollHeight;
    document.querySelector(".content").scrollTop = height;
}

function robotSend(jsontext) {
    console.log(jsontext)
    json = JSON.parse(jsontext)
    for (let i in json.msg) {
        let text = json.msg[i]
        if (text.length == 0) {
            break;
        }
        createNewBubble(text, 0);
    }
    //滚动条置底
    let height = document.querySelector('.content').scrollHeight;
    document.querySelector(".content").scrollTop = height;
}

function userSendByEnter(event) {
    if (event.keyCode == 13) {
        event.preventDefault();
        userSend();
    }
}