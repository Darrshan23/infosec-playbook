# 🔐 XOR Cipher Implementation

> A lightweight, symmetric encryption technique using the XOR bitwise operation

---

## 📖 What is XOR?

**XOR** (Exclusive OR) is a logical operation that outputs `true` only when inputs differ. In binary terms:

| Input A | Input B | Output |
|:-------:|:-------:|:------:|
| 0 | 0 | 0 |
| 0 | 1 | 1 |
| 1 | 0 | 1 |
| 1 | 1 | 0 |

### The Magic Property

XOR has a unique **self-inverse** property that makes it perfect for encryption:

```
A ⊕ B = C
C ⊕ B = A
```

If you XOR data with a key, then XOR the result with the same key again, you get back the original data. This is the foundation of XOR cipher encryption.

---

## 🎯 Applications

### Where XOR Cipher Shines

- **Network Protocols** - Simple obfuscation in data transmission
- **File Encryption** - Quick encryption for non-sensitive data
- **Game Development** - Save file protection and anti-cheat mechanisms
- **Embedded Systems** - Low-resource encryption for IoT devices
- **CTF Challenges** - Common in capture-the-flag competitions

### ⚠️ Important Limitations

XOR cipher is **NOT cryptographically secure** for sensitive data. It's vulnerable to:
- Frequency analysis attacks
- Known-plaintext attacks
- Pattern recognition

For production security, use established algorithms like AES or RSA.

---

## 💻 Code Explanation

### Overview

This implementation demonstrates a basic XOR cipher in C++ that can both encrypt and decrypt text using a single-character key.

```cpp
#include <cstdio>    // Includes printf
#include <cstring>   // Includes strlen
using namespace std;
```

Standard library includes for I/O operations and string manipulation.

---

### The Encryption/Decryption Function

```cpp
void encryptDecrypt(char inpString[]) {
    char xorKey = 'a';
    int len = strlen(inpString);
    for (int i = 0; i < len; i++) {
        printf("%c", inpString[i]);
        inpString[i] = inpString[i] ^ xorKey;
        printf("%c", inpString[i]);
    }
}
```

**How it works:**

1. **Key Definition** - Uses `'a'` (ASCII 97) as the XOR key
2. **Length Calculation** - Gets string length for iteration
3. **Character Processing** - Loops through each character:
   - Prints original character
   - XORs character with key using `^` operator
   - Prints transformed character

**Key Point:** The same function encrypts AND decrypts because of XOR's self-inverse property.

---

### The Main Function

```cpp
int main() {
    char sample[] = "Darrshan";
    printf("Encrypted String: ");
    encryptDecrypt(sample);
    printf("Decrypted String: ");
    encryptDecrypt(sample);
}
```

**Execution Flow:**

1. Creates sample string "Darrshan"
2. First call to `encryptDecrypt()` - **Encrypts** the string
3. Second call to `encryptDecrypt()` - **Decrypts** back to original

---

## 🔍 Visual Example

Let's trace what happens with the letter **'D'**:

```
Original: 'D' (ASCII 68)  → Binary: 01000100
Key:      'a' (ASCII 97)  → Binary: 01100001
                                    --------
XOR Result:                → Binary: 00100101 → '%' (ASCII 37)

Decrypt: '%' (ASCII 37)   → Binary: 00100101
Key:     'a' (ASCII 97)   → Binary: 01100001
                                    --------
XOR Result:                → Binary: 01000100 → 'D' (ASCII 68)
```

The character returns to its original value after two XOR operations with the same key.

---

## 🚀 Sample Output

```
Encrypted String: D%a r r s h a n 
Decrypted String: %D r a s h a n 
```

Each character is transformed by XORing with 'a', then restored by XORing again.

---

## 🛠️ Compilation & Execution

### Using G++
```bash
g++ XOR-Cipher.cpp -o xor_cipher
./xor_cipher
```

### Using Visual Studio 2022+
```bash
cl XOR-Cipher.cpp
XOR-Cipher.exe
```

**Note:** The commented line `#include<bits/stdc++.h>` is a non-standard header available in GCC but not in MSVC (Visual Studio). Use individual headers instead.

---

## 📚 Further Improvements

Want to make this more robust? Consider:

- **Variable-length keys** - Use a string key instead of single character
- **User input** - Allow runtime key and message specification
- **File encryption** - Read from and write to files
- **Better output** - Separate display logic from encryption logic
- **Error handling** - Add bounds checking and validation

---

## 📝 License

This is a demonstration implementation for educational purposes.

---

<div align="center">

**Made with 🔐 and ⊕**

*Remember: Never use XOR cipher for sensitive data in production!*

</div>
