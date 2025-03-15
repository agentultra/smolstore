# smolstore #

Most databases store _current state_.

_Event sourcing_ is a method of persisting _state changes_ rather than
current state.  Store events, facts, whatever you want to call them.
We'll call them, "events."

In essence most of what you need to implement an event sourcing
database is covered by two important operations:

- `create`: create a new stream.
- `append`: add a new event to the end of a stream.
- `seek`: random access to data in the file.

This project is an attempt to see how small, in terms of code, one
could implement a reasonable event-sourcing database.

## Goals ##

The primary goal is to understand what is the _essential_ nature of
event sourcing.  What are the minimum required atomic operations
needed to implement such a database?

### Small ###

We want to use as little code as possible and as few data structures
as possible.

The operating system should provide most of what is needed.

### Zero Dependencies ###

No external dependencies.

### Safe ###

> There are two methods in software design.  One is to make the program
> so simple, there are obviously no errors.  The other is to make it so
> complicated, there are no obvious errors.

Tony Hoare was spot on here.  A side effect of small code is that we
can root out errors.

No memory safety errors: no use-after-free, no out-of-bounds access,
etc.

### Portable ###

For now, this is a research project.  It is not a goal to support
every OS available.  The focus is on POSIX APIs.  However it should be
easy to port to other platforms.

Write all code in standard compliant C23.  That is the portability we
aim for.

### Zero Tech Debt ###

Always compile with all warnings on and never ignore a warning.

If there's a better way to do something, keeping in mind the spirit of
the project, do it.  Especially if it means deleting code.

Fuzz testing, simulation testing, static analysis -- use it all.

### Let the OS Do It ###

If you don't have to, don't.  Leverage the OS as much as possible.

### Minimize Effectful Code ###

Don't use `malloc` outside of `main`.  Or at all if you can avoid it.
Use explicit allocators.  Manage all resources and dependencies in
`main`.

Everything internal is pure C code with no dependencies on the outside
world.

## Requirements ##

### Definitions ###

#### Event ####

Data stored by the user.  Each event can be referred to be its index
in the stream it belongs to.

#### Stream ####

A partially ordered collection of events with a unique identifier.

### Constraints ###

Events must maintain a partial order within a stream.

Events are persisted in the logical order in which they were received.

### Horizon ###

A position in a stream from which events will be read.

Can be one of:

- _beginning_: index 0 of the stream
- _index_ n: start at index _n_
- _newest_: the last event recorded

### Behaviours ###

#### Insert ####

Batch insert at least one event into a stream.

Does nothing if no events are given.

Successful completion of the operation guarantees events are written
to the store.

#### Query ####

Retrieve event data from the database.

Takes a stream identifier, a horizon.

Returns the specified stream of event data.
