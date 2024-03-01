let touch_ids = {left: 0, right: 0};
const brake_area = {left: [0.0, 0.2], right: [0.8, 1.0]}

function send_value(value) {
    const xhr = new XMLHttpRequest();
    xhr.open("POST", "/index.html");
    xhr.setRequestHeader("Content-Type", "text/plain");
    xhr.send(value);
    // console.log(value);
}

const clamp = (n, min, max) => Math.min(Math.max(n, min), max)

function draw_controll_bar(elm, y, brake = false) {
    ctx = elm.getContext('2d')
    ctx.clearRect(0, 0, elm.width, elm.height);

    ctx.fillStyle = "#A0A0A0"
    ctx.fillRect(0, y - 5, elm.width, 10);

    const ba = brake_area[elm.id];
    ctx.fillStyle = brake ? "#FF0000FF" : "#FF000080"
    ctx.fillRect(ba[0] * elm.width, 0, ba[1] * elm.width, elm.height);
}

function calc_local_pos(elm, touch) {
    const cr = elm.getBoundingClientRect();
    const x = clamp(touch.pageX - cr.x, 0, elm.width);
    const y = clamp(touch.pageY - cr.y, 0, elm.height);
    return [x, y]
}

function down(elm, ev) {
    for(let i = 0; i < ev.changedTouches.length; i += 1) {
        const t = ev.changedTouches[i];
        if(t.identifier != touch_ids[elm.id]) {
            continue;
        }

        const [x ,y] = calc_local_pos(elm, t);
        const sym = elm.id[0].toUpperCase()

        const ba = brake_area[elm.id];
        if(x >= ba[0] * elm.width && x < ba[1] * elm.width) {
            send_value(sym + "B");
            draw_controll_bar(elm, elm.height / 2, true);
            return;
        }

        const v = (y - elm.height * 0.5) / elm.height * 2 * -100;
        send_value(sym + v.toString());
        draw_controll_bar(elm, y);
    }
}

function up(elm, ev) {
    for(let i = 0; i < ev.changedTouches.length; i += 1) {
        const t = ev.changedTouches[i];
        if(t.identifier != touch_ids[elm.id]) {
            continue;
        }
        const y = elm.height / 2;
        const sym = elm.id[0].toUpperCase()
        send_value(sym + "C");
        draw_controll_bar(elm, y);
    }
}

function on_touch_down(ev) {
    touch_ids[this.id] = ev.changedTouches[0].identifier;
    down(this, ev);
}

function on_touch_up(ev) {
    up(this, ev);
    touch_ids[this.id] = 0;
}

function on_touch_move(ev) {
    down(this, ev);
}

function init_canvas(name) {
    const canvas = document.getElementById(name);
    const ctx    = canvas.getContext('2d');

    canvas.addEventListener('touchstart', on_touch_down);
    canvas.addEventListener('touchend', on_touch_up);
    canvas.addEventListener('touchmove', on_touch_move);
}

let frame_count = 1;
let last_frame = 1;
let last_update = Date.now();
function update_monitor() {
    const monitor = document.getElementById("monitor_img");
    const src = monitor.src.substring(0, monitor.src.lastIndexOf("/")) + "/snapshot.jpg?_=" + frame_count;
    monitor.src = src;
    frame_count += 1;

    const now = Date.now();
    if(now - last_update >= 1000) {
        document.getElementById("fps").textContent = "fps: " + (frame_count - last_frame).toString();
        last_frame = frame_count;
        last_update = now;
    }
}

function fit_canvas(canvas) {
    canvas.setAttribute("width", canvas.clientWidth);
    canvas.setAttribute("height", canvas.clientHeight);
}

let monitor_update_interval = 33;

function init() {
    init_canvas("left");
    init_canvas("right");

    window.onresize = function () {
        fit_canvas(document.getElementById("left"));
        fit_canvas(document.getElementById("right"));
    }
    window.onresize()

    // setup monitor
    const monitor = document.getElementById("monitor_img");
    function set_monitor_hook(monitor_update_interval) {
        monitor.onload = function() {
            setTimeout(update_monitor, monitor_update_interval);
        };
        monitor.onerror = function() {
            setTimeout(update_monitor, monitor_update_interval);
        };
    }

    const monitor_interval_value = document.getElementById("monitor_interval_value");
    const monitor_interval_apply = document.getElementById("monitor_interval_apply");
    function apply_monitor_interval() {
        const num = Number(monitor_interval_value.value);
        if(num == NaN) {
            return;
        }
        monitor_update_interval = num;
        monitor_interval_apply.disabled = true;
        set_monitor_hook(monitor_update_interval);
    }
    apply_monitor_interval();

    monitor_interval_value.oninput = (event)=> {
        const num = Number(event.target.value);
        monitor_interval_apply.disabled = num == NaN && monitor_update_interval == num;
    };
    monitor_interval_value.onchange = apply_monitor_interval;
    monitor_interval_apply.onclick = apply_monitor_interval;
}

document.addEventListener("DOMContentLoaded", init);
