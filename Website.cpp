#include "Website.h"

void handle_OnConnect(float waterlevel, bool LED, String waterstatus) {
    server.send(200, "text/html", SendHTML(waterlevel, LED, waterstatus));
}
void handle_NotFound() {
    server.send(404, "text/plain", "Not found");
}

String SendHTML(float waterlevel, bool LED, String waterstatus){
    String LED_state;
    String LED_color;
    
    
    if(LED == true){
        LED_state = "ON"; 
    }
    else{
        LED_state = "OFF"; 
    }
    
    if(LED_state == "ON") LED_color = "#cf5953";
    else LED_color = "green"; 

    String ptr = "<!DOCTYPE html>";
    ptr += "<html>";
    ptr += "<head>";

    ptr += "<title>";
    ptr += "Water Monitoring System";
    ptr += "</title>";

    ptr += "<meta name='viewport' content='width=device-width,initial-scale=1.0'>";
    ptr += "<link href='https://fonts.googleapis.com/css?family=Open+Sans:300,400,600' rel='stylesheet'>";
    ptr += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.1/css/all.min.css'>";
    ptr += "<meta http-equiv='refresh' content='5'>"; 
    ptr += "</head>";

    // 2. Phần STYLE
    ptr += "<style>";
    ptr += "html {font-family: 'Open Sans', sans-serif; display:block; margin:0px auto; text-align:center; color:#444444;}";
    ptr += "body{margin: 10px;}";
    ptr += "h1 {margin: 20px auto 10px; color: #007bff;}"; 
    ptr += "h3 {margin: 10px auto 20px;}";

    ptr += ".side-by-side{display: table-cell; vertical-align: middle; position: relative;}";
    ptr += ".text{font-weight: 600;font-size: 19px;width: 200px;}";
    ptr += ".reading{font-weight: 300;font-size: 50px;padding-right: 25px;}";

    // Định dạng màu cho từng chỉ số
    ptr += ".waterlevel .reading{color: #3B97D3;}"; 
    ptr += ".LED .reading{color: " + LED_color + ";}"; // Dùng biến đã được sửa tên
    ptr += ".waterstatus .reading{color: ";
    ptr += (waterstatus == "NORMAL" ? "green" : "orange"); 
    ptr += ";}";

    ptr += ".status{padding: 30px;} ";

    ptr += ".superscript{font-size: 17px;font-weight: 600;position: absolute;top: 10px;}";
    ptr += ".data{padding: 15px; border-bottom: 1px solid #eee;}"; 
    ptr += ".container{display: table;margin: 20px auto; border: 3px solid #007bff; border-radius: 10px; background-color: #f9f9f9;}";
    ptr += ".icon{width:65px; text-align: center;}";

    ptr += "</style>";

    // 3. phần body và khối dữ liệu chính
    ptr += "<body>";

    // Header chính và hình ảnh
    ptr += "<h1>Water Monitoring System</h1>";
    ptr += "<h3>Current Status - Web server IP</h3>";

    // Ảnh minh họa hệ thống điều khiển mực nước
    ptr += "<div class='sub-header'>";
    ptr += "<img width='400px' height='300px' src='https://accautomation.ca/wp-content/uploads/2019/10/Click-PLC-PID-using-Factory-IO-010-min.jpg'>";
    ptr += "</div>";

    ptr += "<div class='container'>";

    // --- KHỐI DỮ LIỆU 1: MỰC NƯỚC (Water_Level) ---
    ptr += "<div class='data water-level'>";
    ptr += "<div class='side-by-side icon'><i class='fas fa-water fa-3x' style='color:#3B97D3;'></i></div>";
    ptr += "<div class='side-by-side text'>Water Level</div>";
    ptr += "<div class='side-by-side reading'>";
    ptr += String(waterlevel, 1); 
    ptr += "<span class='superscript'>%</span></div>";
    ptr += "</div>";

    // --- KHỐI DỮ LIỆU 2: TRẠNG THÁI LED (LED_status) ---
    ptr += "<div class='data led-state'>";
    ptr += "<div class='side-by-side icon'><i class='fas fa-lightbulb fa-3x' style='color:" + LED_color + ";'></i></div>";
    ptr += "<div class='side-by-side text'>LED Status</div>";
    ptr += "<div class='side-by-side reading'>";
    ptr += LED_state;
    ptr += "</div>";
    ptr += "</div>";

    // --- KHỐI DỮ LIỆU 3: TRẠNG THÁI WIFI (WiFi_status) ---
    ptr += "<div class='side-by-side icon'><div class='data water-status'>";
    ptr += "<div class='fas fa-jar fa-3x' style='color:";

    ptr += (waterstatus == "NORMAL" ? "green" : "orange"); 
    ptr += ";'></i></div>";
    ptr += "<div class='side-by-side text status'>";
    ptr += "Water Status";
    ptr += "</div>";

    ptr += "<div class='side-by-side reading'>";
    ptr += waterstatus;
    ptr += "</div>";

    ptr += "</div>";

    ptr += "</div>"; 

    ptr += "<script>";
    ptr += "setInterval(function(){";
    
    // 1. Tạo đối tượng XMLHttpRequest
    ptr += "var xhttp = new XMLHttpRequest();";
    
    // 2. Định nghĩa hàm xử lý khi có phản hồi từ server
    ptr += "xhttp.onreadystatechange = function(){";
    ptr += "if (this.readyState == 4 && this.status == 200){";
    
    // Dữ liệu trả về (responseText) phải là một chuỗi định dạng CSV: waterlevel, LED_state, waterstatus
    ptr += "var readings = this.responseText.split(',');";
    
    // Cập nhật giá trị Mực nước (Water Level)
    ptr += "document.getElementsByClassName('water-level')[0].getElementsByClassName('reading')[0].innerHTML = readings[0] + \"<span class='superscript'>%</span>\";";
    
    // Cập nhật trạng thái LED
    ptr += "document.getElementsByClassName('led-state')[0].getElementsByClassName('reading')[0].innerHTML = readings[1];";
    
    // Cập nhật Trạng thái Nước (Water Status)
    ptr += "document.getElementsByClassName('water-status')[0].getElementsByClassName('reading')[0].innerHTML = readings[2];";
    
    ptr += "}";
    ptr += "};";
    
    // 3. Mở kết nối GET tới endpoint /readings và gửi request
    ptr += "xhttp.open(\"GET\", \"/readings\", true);";
    ptr += "xhttp.send();";
    
    
    ptr += "}, 500);"; 
    ptr += "</script>";
    ptr += "</body>";
    ptr += "</html>";
    return ptr;
}