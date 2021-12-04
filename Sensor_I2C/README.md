# Sensor_I2C (Down kèm nếu muốn sử dụng các thư viện cảm biến trong kho)
## 1 thư viện Arduino cho các cảm biến có giao tiếp I2C.

Thư viện này được tạo ra nhằm mục đích làm tiền đề cho các thư viện về từng cảm biến cụ thể. Các thư viện cảm biến sẽ tạo lớp là lớp con của lớp `SENSOR`, kế thừa 4 hàm đọc viết I2C cơ bản. Ngoài ra, thư viện còn cung cấp namespace `I2C` chứa những hàm quét địa chỉ cảm biến ngắn gọn.

#### Thư viện cung cấp 5 hàm chung cho việc quét địa chỉ cảm biến, sử dụng namespace `I2C`:
- Quét địa chỉ các cảm biến trên bus I2C, lưu vào mảng `addr[]` truyền vào, trả về số lượng địa chỉ xác định được. `maxDevices` quyết định số cảm biến tối đa muốn quét, quét đủ sẽ kết thúc hàm ngay. Nếu không muốn mảng bị ghi giá trị vào, cho giá trị biến `justNumber` thành **true** (1).
```
  uint8_t scanAllDevices(uint8_t addr[], uint8_t maxDevices = 128, bool justNumber = 0);
```
- Quét địa chỉ cảm biến theo thứ tự địa chỉ tăng dần, trả về địa chỉ cảm biến theo đúng thứ tự.
```
  uint8_t scanDeviceByOrdinal(uint8_t ordinal = 1);
```
- Quét số địa chỉ cảm biến trên bus I2C, trả về số lượng địa chỉ xác định được.
```
  uint8_t numberOfDevices();
```
- Kiểm tra cảm biến có địa chỉ truyền vào có gặp lỗi khi truyền nhận trên bus I2C hay không.
Các giá trị trả về có thể là:
  0 : kết nối tốt
  2 : địa chỉ đã được gửi đi, không nhận được phản hồi (_NACK_), nếu không có cảm biến ứng với địa chỉ sẽ cho ra kết quả này
  4 : các lỗi khác
```
  uint8_t checkDeviceConnection(const uint8_t &addr);
```
- Xóa buffer, là mảng lưu dữ liệu đọc được trong thư viện Wire.h của Arduino.
```
  void flush();
```

#### Phần chính của thư viện này nằm ở lớp `SENSOR`. Lớp này lưu địa chỉ cảm biến, địa chỉ thanh ghi ID (hoặc thanh ghi cố định nào đó) của cảm biến, giá trị ID và tên cảm biến do người dùng đặt.
+ Constructor yêu cầu tên cảm biến ở dạng mảng char và địa chỉ thanh ghi ID.
```
  SENSOR(const char *deviceName, const uint8_t &idAddr);
```
+ Khởi đầu với một hàm quét địa chỉ hoặc kiểm tra địa chỉ người dùng nhập vào, bắt buộc trả về **true**, nếu không sẽ kẹt lại trong vòng lặp quét địa chỉ cảm biến.
```
  bool sensorBegin(uint8_t addr = 255, uint8_t ordinal = 1);
```
+ Hàm `showInfo()` in thông tin cảm biến (tên, địa chỉ, ID) ra serial. Hàm `getName()` trả về tên cảm biến dạng `char*`. Hàm `getId()` trả về ID cảm biến dạng `byte`.
+ 4 hàm đọc ghi quan trọng nhất (`failAlt` là giá trị trả về khi đọc cảm biến thất bại):
```
  bool readBytes(uint8_t bytes[], const uint8_t &numberOfBytes, uint8_t failAlt = 0);	// Read a number of bytes
  uint8_t readByte(const uint8_t &registerAddr, uint8_t failAlt = 0);	// Read a byte
  uint16_t readWord(const uint8_t &registerAddr, uint16_t failAlt = 0);	// Read 2 bytes
  bool writeByte(const uint8_t &registerAddr, const uint8_t &cwd); // Write a byte to a register
```
**KẾT THÚC**
