```mermaid
classDiagram

    Platform : -U n_threads
    Platform : -U n_sims
    Platform : +Platform(threads, sims, config)
    Platform : +simulate()
   
    SlotConfig : +SlotConfig(filename)

    Game : -U n_sims
    Game : -vector~SlotState~
    Game : +Game(SlotConfig)
    Game : +play()
    Game : -_round()

    Slot : -slot_t _slot
    Slot : -stop_t _stops
    Slot : -rows_t n_rows
    Slot : +Slot()

    Spinner : +Slot slot
    Spinner : +get_slot() slot_t
    Spinner : +get_stops() stops_t
    Spinner : +get_rows() rows_t
    Spinner : +fill(sym_t) void
    Spinner : +clear() void
    Spinner : +replace(from, to, symbol) void
    Spinner : +replace(index, axis, symbol) void
    Spinner : +count(sym_t) int
    Spinner : +find(sym_t) vector~pair~column, row~~
    Spinner : +Spinner()

    HoldAndWinSpinner : +HoldAndWinSpinner()
    HoldAndWinSpinner : +spin(reels, rows, stops)
    StandardSpinner : +StandardSpinner()
    StandardSpinner : +spin(reels, rows, stops)


    %% SlotState 
    SlotState : +SlotState()
    SlotState : #unique_ptr~Spinner~
    SlotState : +vector~Trigger~
    StandardState : +StandardState()
    FreespinState : +FreespinState()

    %% SlotLogic : +SlotLogic()
    %% SlotLogic : -vector~Trigger~

    


    Evaluator : +Evaluator()
    Win : +Win()
    
    
    Trigger : +Trigger()
    Trigger : +check() bool

    %% SlotSpinner


    


    Spinner <|-- HoldAndWinSpinner
    Spinner <|-- StandardSpinner
    Spinner --> Slot
    SlotState --> Spinner
    SlotState *-- Trigger
    %% Game --> SlotLogic
    Platform *-- SlotConfig
    %% Game *-- Slot
    Platform --> Game
    SlotState <|-- StandardState
    SlotState <|-- FreespinState
    Game *-- SlotState

```
## Slot
- Only contains the DS.

## Spinner
- Manipulator class of Slot, offers manipulation APIs to spin "recepies".
- `spin(spin_func)` method executes the spinning recepie `spin_func`.

## Platform
- Manages multithreding
- Manages Data aggregation
- Multithreads on `Game`

## Game
- Manages the Slot state machine depending on which trigger was activated
- Returns data to the `Platform`
- `_round()` starts at state 0, travels the state machine until it gets back to 0. Analogously populates the stack until it's empty.
- `play()` calls `_round()` `sim` times.

## Definitions
- `Slot`: a DS containig symbols with utils to generate a new slot, and manipulation
- `Platform`: Manages the concurrency
- `Game`: Necessary middleware to handle the `Slot`'s state. Applies `Triggers` and evaluates `Slot` using `Evaluators`
- `Triggers`
- `Evaluators`


## SlotConfig
The `.json` specifies the states of the machine. The triggers are coded.
```json
{
    "states": [
        0,
        1
    ],
    "slots": {
        "0": {
            "reels": 5,
            "rows": 3,
            "symbols": [1,2,3,4,5,6,7,8,9],
            "paylines": {
                "0": [2,2,2,2,2],
                "1": [0,0,0,0,0],
                "2": [1,1,1,1,1]
            },
            "paytable": {
                "1": [7.50,2.5,1,5],
                "2": [3.75,2,1]
            }
        },
        "1":{
           "Other state":1
        }
    }
}
```



## Decisions
- Keep stuff private and add fuctionality that can be tested.
- Configuration starts by numbering the slots and their interaction




## TODO
- Cascade Feature



