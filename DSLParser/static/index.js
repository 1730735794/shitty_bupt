window.onload = function init() {
    document.title = "test"
    let date = new Date();
    let text = date.toLocaleString();
    let timeStamp = document.createElement('div');
    let hello = document.createElement('div');
    timeStamp.className = 'item item-center';
    timeStamp.innerHTML = `<span>${text}</span>`;
    hello.className = 'item item-center';
    hello.innerHTML = `<span>会话已开始</span>`;
    let firstChild = document.querySelector('.content').firstChild;
    if (firstChild) {
        document.querySelector('.content').insertBefore(timeStamp, firstChild);
        document.querySelector('.content').insertBefore(hello, firstChild);
    }
    else {
        document.querySelector('.content').appendChild(timeStamp);
        document.querySelector('.content').appendChild(hello);
    }
    document.querySelector('#textarea').value = '';
    document.querySelector('#textarea').focus();
}

function userSend() {
    let text = document.querySelector('#textarea').value;
    if (!text) {
        alert('请输入内容');
        return;
    }
    let item = document.createElement('div');
    item.className = 'item item-right';
    item.innerHTML = `<div class="bubble bubble-left">${text}</div><div class="avatar"><img src="static/me.jpg" /></div>`;
    document.querySelector('.content').appendChild(item);
    document.querySelector('#textarea').value = '';
    let options = {
        method: 'POST',
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({ msg: text })
    };
    fetch("/reply", options).then(resp => resp.text()).then(robotSend);
    document.querySelector('#textarea').focus();
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

function robotSend(jsontext) {
    json = JSON.parse(jsontext)
    for (let i in json.msg) {
        let text = json.msg[i]
        if (text.length == 0) {
            let end = document.createElement('div');
            end.className = 'item item-center';
            end.innerHTML = `<span>会话已结束</span>`;
            document.querySelector('.content').appendChild(end);
            break;
        }
        let item = document.createElement('div');
        item.className = 'item item-left';
        item.innerHTML = `<div class="avatar"><img src="static/robot.jpg" /></div><div class="bubble bubble-left">${text}</div>`;
        document.querySelector('.content').appendChild(item);
    }
    //滚动条置底
    let height = document.querySelector('.content').scrollHeight;
    document.querySelector(".content").scrollTop = height;
}