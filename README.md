# 📦 Lost and Found Item Management System (C Project)

A simple terminal-based Lost and Found management system written in C. This program allows users to report lost or found items, and provides an admin interface to manage the records. It's a beginner-friendly project demonstrating the use of structures, file handling, and console UI customization in C.

## 📜 Features

- Add lost or found item details.
- View all reported items.
- Search for specific items by name.
- Delete items (admin only).
- Update item status (admin only).
- Admin authentication system.
- Colorful console UI using ANSI color codes.
- Simple loading animations for a better UX.

## 📂 Project Structure

```
.
├── PHASE2_001.c     # Main C program file
├── README.md        # Project documentation
```

## 🛠️ How to Compile and Run

### 📌 Requirements:
- GCC Compiler (e.g., `gcc` on Linux/Windows with MinGW)

### 📌 Compile:
```bash
gcc PHASE2_001.c -o lost_and_found
```

### 📌 Run:
```bash
./lost_and_found
```

## 🔒 Admin Credentials

| Username | Password |
|:----------|:------------|
| `admin`   | `password`  |

**Note:** You should change the default password in the source code for better security.

## 📝 Code Highlights

- Uses **structs** to manage item data.
- File I/O for persistent data storage (if implemented — modify code to add this).
- ANSI color codes for terminal styling.
- Platform-specific includes for cross-platform compatibility.

## 📸 Demo

*Sample terminal interface screenshot (add yours here)*

## 📌 Future Improvements

- Implement file-based data storage.
- Allow item image attachments (file paths).
- Date validation for inputs.
- Improved password management (hashed or encrypted).

## 📄 License

This project is open-source and available under the [MIT License](LICENSE).
