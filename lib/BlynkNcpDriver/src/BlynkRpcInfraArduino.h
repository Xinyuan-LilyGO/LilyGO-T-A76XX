#warning "BlynkRpcInfraArduino.h is deprecated and will be removed"

int rpc_uart_available() {
  int result = SerialNCP.available();
  if (!result) { yield(); }
  return result;
}
int rpc_uart_read() {
  return SerialNCP.read();
}
size_t rpc_uart_write(uint8_t data) {
  return SerialNCP.write(data);
}
void rpc_uart_flush() {
  SerialNCP.flush();
}
uint32_t rpc_system_millis() {
  return millis();
}
