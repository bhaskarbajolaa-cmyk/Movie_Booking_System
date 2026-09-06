# Movie Ticket Booking System

A console-based application for booking movie tickets built in C++. This project demonstrates core Object-Oriented Programming (OOP) concepts and SOLID principles.

## Features
- List available movies and view real-time seat layouts.
- Book multiple seats in different categories (Silver, Gold, Platinum).
- Calculate prices based on seat category.
- Payment processing simulation (UPI, Card, Cash).
- Generate and print formatted tickets.
- Cancel bookings and process refunds.

## How to Run
1. Ensure you have a C++ compiler installed (like g++).
2. Open a terminal in the project directory.
3. Compile the source code:
   ```bash
   g++ main.cpp -o app
   ```
4. Run the executable:
   ```bash
   ./app
   ```
   *(On Windows, use `.\app.exe`)*

## Architecture
The system is built with modularity in mind. Each class resides in its own file.
Core entities include `Movie`, `Seat`, `Screen`, `Cinema`, `Show`, `ShowSeat`, `Customer`, and `Booking`. Service and behavior logic is separated into `PriceCalculator`, `TicketPrinter`, and `BookingService`.
