# C++ Video Event Processing with Docker — Distributed Broker System

🇷🇺 *Если ищешь русскую версию смотри ->* [README.md](README.md)

# StreamBroker — A Lightweight Message Broker in C++

## Description

This project focuses on distributed stream data processing, modeled after a video hosting platform. The goal is to build a simplified version of a message broker system inspired by Apache Kafka and RabbitMQ, implemented in C++ and containerized using Docker.

### Concept

The system simulates the behavior of a real-world video hosting service, where millions of users interact with video content and generate a stream of metadata events. These events are passed through a custom-built message broker, processed, logged, and persisted for further use.

### Architecture

The project follows a microservice architecture and includes the following components (illustrated in the diagram below):

- **Producers** — services that simulate user actions. They generate events such as:
  - Video views
  - Likes/dislikes
  - Comments
  - Watch duration
  - Metadata and timestamps

- **Broker** — the central component:
  - Uses an in-memory ring buffer to store messages
  - Enforces queue size limits and blocks writing when full
  - Blocks writing if no active consumers are present
  - Periodically saves its state (checkpoints) to a persistent database

- **Consumers** — services that read events from the broker and:
  - Collect and display statistics
  - Aggregate metrics
  - Provide real-time analytics for content creators

- **Redis** — used for real-time logging and event monitoring.

- **SQLite** — persistent storage used by the broker to save checkpoints for crash recovery.

![Project Architecture](misc/architecture.png)

### Load Simulation

The system emulates a simplified video hosting workload:

- **1,000 videos**
- **10 content creators**
- **1,000,000 users**
- Users interact with videos at random, generating tens of millions of events

This allows us to test how the broker handles concurrent writes, buffer overflows, and state persistence under real-world-like load.

### Project Objectives

- Learn the principles behind distributed message queue systems
- Implement multithreaded synchronization and communication in C++
- Simulate a real data pipeline for a video platform
- Containerize all services using Docker for modularity and scalability

## Technical Requirements

1. **Message Queue**  
   Implement a message channel with queue-like behavior and a size limit. Writing must be blocked when the queue is full.

2. **Read/Write Interface**  
   Provide a class or library for writing to and reading from the queue. Support templates for generality.

3. **Ring Buffer**  
   Implement the queue as an in-memory ring buffer.

4. **Blocking on No Consumers**  
   In dynamic-size mode (no buffer limit), if no consumer is available, writing should be blocked until a reader appears.

5. **Iterator Interface**  
   Provide access to the queue using a custom iterator (`iter`, `next`) for stream-like consumption of data.

6. **State Persistence (Checkpoints)**  
   Periodically dump the queue state to disk (SQLite) to enable recovery after crashes.

7. **Docker Containerization**  
   All components (Producers, Broker, Consumers, Redis, SQLite) must run in separate containers managed via `docker-compose`.

8. **Logging with Redis**  
   All events (incoming, outgoing, errors, and metadata) are logged in Redis for monitoring and debugging.

9. **Concurrency and Thread Safety**  
   Ensure correct behavior during concurrent reads/writes with protection against race conditions and deadlocks.

## Microservice Descriptions

### 1. Producer

**Role:** Simulates user behavior on the video platform.

**Functionality:**
- Generates random events: views, likes, comments, watch duration, etc.
- Sends messages to the broker via TCP or IPC.
- Can be launched in multiple instances to simulate heavy load.

**Event Format Example:**
```json
{
  "user_id": 158293,
  "video_id": 731,
  "event": "like",
  "timestamp": "2025-05-13T14:12:00Z"
}
```

### 2. Broker
**Role:** Core message queue system.

**Functionality:**
- Stores events in a ring buffer.
- Controls buffer size and blocks producers when full.
- Handles multithreaded synchronization between producers and consumers.
- Exposes iterator interface (`iter`, `next`) for data consumption.
- Persists state (checkpoints) to SQLite.
- Logs all events to Redis.
- Blocks writes if no consumers are currently active (in unbounded mode).

### 3. Consumer
**Role:** Receives messages from the broker and processes them.

**Functionality:**
- Reads events using the broker’s iterator.
- Processes data: counts views, likes, evaluates watch duration, etc.
- Outputs aggregated data to stdout or external storage.
- Can be horizontally scaled with multiple containers.

### 4. Redis Logger
**Role:** Centralized logging for the entire system.

**Functionality:**
- Collects logs from all services.
- Categorizes events (incoming, outgoing, errors).
- Can be integrated with external monitoring tools like Grafana.

### 5. SQLite Storage
**Role:** Persistent storage for broker state.

**Functionality:**
- Stores ring buffer checkpoints (offsets, head/tail positions).
- Allows state recovery after service restarts or crashes.
- Supports periodic dumps either by time or message count.

