# Movie Ticket Booking System

A console-based application for booking movie tickets built in C++. This project demonstrates core Object-Oriented Programming (OOP) concepts and SOLID principles.

## Functional Requirements (FR)
- **FR1 — Browsing**: a customer views a list of all movies currently playing. Each movie displays its title, language, and duration.
- **FR2 — Shows**: for a selected movie, the system displays all available shows, including the screen number and start time.
- **FR3 — Seat Layout**: for a selected show, the system prints a categorized seat layout (SILVER, GOLD, PLATINUM) with each seat marked clearly as AVAILABLE or BOOKED.
- **FR4 — Booking**: a customer selects one or more seat numbers for a show. If any selected seat is already BOOKED, the whole booking is rejected and no seat changes state. Booking is confirmed only after payment succeeds.
- **FR5 — Pricing**: the total booking amount is dynamically calculated based on seat types: SILVER (₹150), GOLD (₹250), PLATINUM (₹400).
- **FR6 — Payment**: exactly one method (UPI / Card / Cash) per booking. If payment fails, seats are released and booking status becomes FAILED.
- **FR7 — Ticketing**: after successful payment, a formatted ticket is printed showing booking ID, movie name, screen, time, booked seat numbers, total amount, and CONFIRMED status.
- **FR8 — Cancellation**: a customer cancels an existing CONFIRMED booking. The associated seats immediately revert to AVAILABLE status and the payment is refunded.

## Non-Functional Requirements (NFR)
- **NFR1 — Modularity**: The system must enforce strict modularity by keeping exactly one class per file without using header files.
- **NFR2 — Extensibility**: The payment system must be open for extension but closed for modification, meaning adding NetBanking must not require edits to the existing `BookingService` class.
- **NFR3 — Robustness**: The system must validate bad input (like invalid seat numbers or incorrect menu choices) gracefully by displaying a clear message without crashing.

## Architecture & Diagrams
The system is built with modularity in mind. Each class resides in its own file.
Core entities include `Movie`, `Seat`, `Screen`, `Cinema`, `Show`, `ShowSeat`, `Customer`, and `Booking`. Service and behavior logic is separated into `PriceCalculator`, `TicketPrinter`, and `BookingService`.

### UML Class Diagram
```mermaid
classDiagram
    class Cinema {
        -String name
        -vector~Screen*~ screens
        +addScreen(Screen*) void
        +listMovies() vector~Movie*~
        +findShows(Movie*) vector~Show*~
        +scheduleShow(Show*) void
        +getScreens() vector~Screen*~
    }

    class Screen {
        -int screenNo
        -vector~Show*~ shows
        -vector~Seat*~ seats
        +getScreenNo() int
        +getSeats() vector~Seat*~
        +getShows() vector~Show*~
        +addSeat(Seat*) void
        +addShow(Show*) void
    }

    class Seat {
        -int seatNo
        -String seatType
        +getSeatNo() int
        +getSeatType() String
    }

    class Movie {
        -String title
        -int durationMin
        -String language
        +getTitle() String
        +getDurationMin() int
        +getDetails() String
    }

    class Show {
        -int showId
        -Movie* movie
        -Screen* screen
        -vector~ShowSeat*~ showSeats
        -String startTime
        +getShowId() int
        +getMovie() Movie*
        +getScreen() Screen*
        +getStartTime() String
        +getShowSeats() vector~ShowSeat*~
        +addShowSeat(ShowSeat*) void
    }

    class ShowSeat {
        -String status
        -Seat* seat
        +isBooked() bool
        +bookSeat() void
        +unBookSeat() void
        +getSeat() Seat*
        +getStatus() String
    }

    class Customer {
        -String name
        -String phoneNo
        +Customer(String name, String phoneNo)
        +Customer(String name)
        +getName() String
        +getPhoneNo() String
    }

    class BookingService {
        -map~int, Booking*~ bookings
        -map~Customer*, Booking*~ customerBookings
        -static int nextBookingId
        +createBooking(Customer*, Show*, vector~ShowSeat*~, float) Booking*
        +getBooking(Customer*) Booking*
        +getBooking(int) Booking*
        +getBookingId(Customer*) int
        +bookingProcess(Customer*) void
        +processPayment(Booking*, Payment*) bool
        +cancelBooking(int) bool
    }

    class Booking {
        -int bookingId
        -vector~ShowSeat*~ bookedSeats
        -Customer* customer
        -Show* show
        -String status
        -Payment* payment
        -float totalAmount
        +getBookingId() int
        +confirmBooking() void
        +cancelBooking() void
        +failBooking() void
        +getTotalAmount() float
        +getCustomer() Customer*
        +getShow() Show*
        +getBookedSeats() vector~ShowSeat*~
        +getStatus() String
        +setPayment(Payment*) void
        +getPayment() Payment*
    }

    class Payment {
        <<abstract>>
        +pay(float amount)* bool
    }

    class Refund {
        <<interface>>
        +refund(float amount)* bool
    }

    class UPIPayment {
        -String upiId
        +UPIPayment(String)
        +pay(float) bool
        +refund(float) bool
    }

    class CardPayment {
        -String cardNumber
        +CardPayment(String)
        +pay(float) bool
        +refund(float) bool
    }

    class CashPayment {
        +pay(float) bool
    }

    class PriceCalculator {
        -float SILVER_PRICE
        -float GOLD_PRICE
        -float PLATINUM_PRICE
        +calculateSeatPrice(Seat*) float
        +calculateBookingTotal(vector~ShowSeat*~) float
    }

    class TicketPrinter {
        +printTicket(Booking*) void
    }

    Cinema "1" *-- "1..*" Screen : Composition
    Screen "1" *-- "1..*" Seat : Composition
    Show "1" o-- "1" Movie : Aggregation
    Show "0..*" --> "1" Screen : Association
    Show "1" *-- "1..*" ShowSeat : Composition
    ShowSeat "1..*" o-- "1" Seat : Aggregation
    
    BookingService "1" *-- "0..*" Booking : Composition
    Booking "0..*" o-- "1" Customer : Aggregation
    Booking "0..*" o-- "1" Show : Aggregation
    Booking "1" --> "1..*" ShowSeat : Association
    Booking "1" --> "1" Payment : Association

    Payment <|-- UPIPayment : Inheritance
    Payment <|-- CardPayment : Inheritance
    Payment <|-- CashPayment : Inheritance
    
    Refund <|.. UPIPayment : Realization
    Refund <|.. CardPayment : Realization
    
    TicketPrinter ..> Booking : Dependency
    PriceCalculator ..> ShowSeat : Dependency
    PriceCalculator ..> Seat : Dependency
```

### Sequence Diagram: "Customer books 1 seat and pays by UPI"
```mermaid
sequenceDiagram
    autonumber
    
    actor Customer as customer
    participant Show as show
    participant ShowSeat as showSeat
    participant PriceCalculator as priceCalculator
    participant BookingService as bookingService
    participant Booking as booking
    participant Payment as payment
    participant TicketPrinter as ticketPrinter

    activate Customer
    Customer->>Show: getShowSeats()
    activate Show
    Show-->>Customer: showSeats
    deactivate Show

    Customer->>PriceCalculator: calculateBookingTotal(selectedSeats)
    activate PriceCalculator
    PriceCalculator->>ShowSeat: getSeat()
    activate ShowSeat
    ShowSeat-->>PriceCalculator: seat details
    deactivate ShowSeat
    PriceCalculator-->>Customer: totalAmount
    deactivate PriceCalculator

    Customer->>BookingService: createBooking(customer, show, selectedSeats, totalAmount)
    activate BookingService
    
    BookingService->>ShowSeat: isBooked()
    activate ShowSeat
    ShowSeat-->>BookingService: false
    deactivate ShowSeat
    
    BookingService->>ShowSeat: bookSeat()
    activate ShowSeat
    ShowSeat-->>BookingService: void
    deactivate ShowSeat
    
    BookingService->>Booking: <<create>> Booking(id, customer, show, seats, totalAmount)
    activate Booking
    Booking-->>BookingService: booking object
    deactivate Booking
    
    BookingService-->>Customer: booking
    deactivate BookingService

    Customer->>Payment: <<create>> UPIPayment("upi@bank")
    activate Payment
    Payment-->>Customer: payment object
    deactivate Payment

    Customer->>BookingService: processPayment(booking, payment)
    activate BookingService
    
    BookingService->>Booking: setPayment(payment)
    activate Booking
    Booking-->>BookingService: void
    deactivate Booking
    
    BookingService->>Payment: pay(totalAmount)
    activate Payment
    Payment-->>BookingService: true (success)
    deactivate Payment
    
    BookingService->>Booking: confirmBooking()
    activate Booking
    Booking-->>BookingService: void
    deactivate Booking
    
    BookingService-->>Customer: true (payment successful)
    deactivate BookingService

    Customer->>TicketPrinter: printTicket(booking)
    activate TicketPrinter
    
    TicketPrinter->>Booking: getStatus()
    activate Booking
    Booking-->>TicketPrinter: "CONFIRMED"
    deactivate Booking
    
    TicketPrinter->>Booking: getBookedSeats()
    activate Booking
    Booking-->>TicketPrinter: selectedSeats
    deactivate Booking
    
    TicketPrinter-->>Customer: void (prints to console)
    deactivate TicketPrinter
    deactivate Customer
```

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
