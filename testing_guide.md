## Testing

### Requirements

You should have installed:

- `docker`
- `docker-compose`

### Run containers

Run **`N`** instances of the program to simulate a network:

`docker-compose up --detach --build --scale host=`**`N`**

### Enable user input from one of running hosts

Run

`docker attach simple_p2p_host_`**`N`**

where **`N`** is a host id.

### Access to container

To access to the container (not to the program user input) to create or read files run:

`docker exec -it simple_p2p_host_`**`N`**` bash`

where **`N`** is a host id.

### Show container logs

Run

`docker logs simple_p2p_host_`**`N`**

where **`N`** is a host id.

### Stop testing
Run

`docker-compose down`

