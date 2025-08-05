
<h1 align="center" style="color:#4CAF50;">
 ⚡ STM32 Flash Memory Driver
</h1>

<p align="center">
  <em>Low-level Flash memory read/write system for STM32 microcontrollers</em><br>
</p>

---
## 🌟 What It Does

This C-based driver enables embedded systems to **store and retrieve non-volatile data** using the STM32 internal Flash memory.

### It supports:

- 📥 Writing 64-bit values to Flash  
- 📤 Reading 64-bit values from Flash  
- 🧹 Erasing Flash pages  
- 🛡️ Performing safe, atomic operations by managing Flash access properly  

---

## 🧠 Core Concepts

The driver handles critical Flash memory tasks, assuming:

- 🔐 Flash memory must be unlocked before any operation  
- 🚫 Interrupts must be disabled during write/erase to avoid corruption  
- ⏳ Timeout mechanisms are essential for preventing system hangs  
- 🛠️ Flash should be re-locked after each operation for protection  

---

## 🔧 Key Features

### 🔁 Flash Memory Operations

- `flash_memory_page_erase()` – clears the Flash page for reuse  
- `write_flash_memory(address, *data)` – stores 64-bit data in Flash  
- `read_flash_memory(address)` – retrieves 64-bit data from Flash  

### 🔒 Safety Mechanisms

- Automatic lock/unlock sequence  
- Flash busy flag monitoring  
- Error flag detection & clearing  
- Interrupt masking during critical write/erase  

---

## 📦 Configuration

- Predefined Flash memory addresses like `FLASH_ADDR_1`  
- Configurable memory layout (via linker script)  
- Reserved Flash section prevents application overwrite  

---

## 🧪 Example Usage

```c
// Example write
uint64_t data = 0xABCD12345678EF90;
write_flash_memory(FLASH_ADDR_1, data);

// Example read
uint64_t read_data = read_flash_memory(FLASH_ADDR_1);
