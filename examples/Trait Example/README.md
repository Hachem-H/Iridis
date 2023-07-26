# Trait Example

In Iridis, traits provide a convenient way to share common properties among structures, promoting code reusability and organization. In this example, we'll demonstrate how to use the `using` keyword to define traits and apply them to different structures.

## Base Entity Structure

Let's start by defining a base `Entity` struct with some common properties:

```iridis
Entity :: struct
{
    x:  i32,
    y:  i32,
    id: i32
}
```

## Printing Entity Data

Next, we can create a procedure called `PrintEntityData` to print the properties of the `Entity` struct:

```iridis
io :: import!("std.io")

PrintEntityData :: proc(entity: Entity)
{
    io.println("Entity Data")
    io.println("--- ENTITY DATA ---")
    io.println("Position: {}, {}", entity.x, entity.y)
    io.println("Entity ID: {}", entity.id)
}
```

## Player Struct - Using the Entity Trait

Now, let's define a `Player` struct that inherits properties from the `Entity` struct using the `using` keyword:

```iridis
Player :: struct using Entity
{
    somePlayerData:      i32,
    someOtherPlayerData: i32
}
```

With this declaration, the `Player` struct will include all the properties from the `Entity` struct, as if they were directly defined in the `Player` struct. It enables the `Player` struct to access `x`, `y`, and `id` fields, just like an `Entity`.

## Printing Player Data

We can also create a procedure called `PrintPlayerData` to print the specific properties of the `Player` struct:

```iridis
io :: import!("std.io")

PrintPlayerData :: proc(player: Player)
{
    io.println("--- PLAYER DATA ---")    
    io.println("Some Data: {}", player.somePlayerData)
    io.println("Some Other Data: {}", player.someOtherPlayerData)
}
```

## Using the Traits

Now, in the `main` procedure, we can demonstrate how to use the `Entity` and `Player` structs:

```iridis
main :: proc()
{
    player := Player 
    {
        x:  0,
        y:  0, 
        id: 0,

        somePlayerData:      67,
        someOtherPlayerData: 21,
    }

    // Since Player has the same properties as Entity, we can call Entity functions on it
    PrintEntityData(player)
    PrintPlayerData(player)
}
```

Since the `Player` struct inherits properties from the `Entity` struct, we can call the `PrintEntityData` procedure on a `Player` instance. Additionally, we can call the `PrintPlayerData` procedure to print the additional properties of the `Player` struct.

Using traits in Iridis allows for a more organized and modular approach in defining structures and sharing properties among them. This enhances code readability and maintainability, making Iridis a powerful language for game development and UI development.

For a more elaborate example, please refer to the `main.iridis` file. Happy coding!
