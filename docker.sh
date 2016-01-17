#!/bin/bash

# Author : gattazr <remi.gattaz@gmail.com>
# Date: 16/01/2016

PROG_NAME=$(basename $0)
CONTAINER_NAME="nachosDev"
CONTAINER_IMAGE="quentinfa/as_ricm4_nachos"
CONTAINER_ARGS="-ti -v $(pwd)/code/:/nachos --workdir='/nachos'"
CONTAINER_COMMAND=""
CONTAINER_ID=""
CONTAINER_STATUS=""

#######################################
# Print the script help
# Arguments:
#     None
# Returns:
#     None
#######################################
print_help(){
    echo -e "Usage: $PROG_NAME [start|stop|shell|delete|status]"
    echo -e "Script managing a docker image run as a daemon container and can start new shell on it\n"

    echo -e "  start  \t\tCreate and start a daemon container. Restart if it was already created."
    echo -e "  stop   \t\tStop the daemon container"
    echo -e "  shell  \t\tStart a shell session on the container"
    echo -e "  remove \t\tRemove the daemon container"
    echo -e "  status \t\tReturn the status of the container"
}


#######################################
# Update the CONTAINER_ID and CONTAINER_STATUS globals
# Globals:
#     CONTAINER_NAME
#     CONTAINER_ID
#     CONTAINER_STATUS
# Arguments:
#     None
# Returns:
#     None
#######################################
get_container_info(){
    wInfoString=$(docker ps -aq --filter "name=$CONTAINER_NAME" --format "{{.ID}}:{{.Status}}")
    IFS=': ' read -r -a wInfos <<< "$wInfoString"
    CONTAINER_ID="${wInfos[0]}"
    CONTAINER_STATUS="${wInfos[1]}"
}

#######################################
# Create and start a docker container. If a container already exists, it is restarted
# Globals:
#     CONTAINER_STATUS
#     CONTAINER_ID
#     CONTAINER_ARGS
#     CONTAINER_NAME
#     CONTAINER_IMAGE
#     CONTAINER_COMMAND
# Arguments:
#     None
# Returns:
#     integer: exit code
#######################################
start_container(){

    # Check if the action can be done
    if [ "$CONTAINER_STATUS" == "Up" ]; then
        echo "container is already running"
        return -1
    fi

    # Create and start container or restart existing one
    if [ "$CONTAINER_STATUS" == "Exited" ]; then
        wAction="restarted"
        echo "Restarting an existing container"
        docker restart $CONTAINER_ID
    else
        wAction="started"
        echo "Creating and starting a new container"
        docker run -d $CONTAINER_ARGS --name=$CONTAINER_NAME $CONTAINER_IMAGE $CONTAINER_COMMAND
    fi

    # Update the container info globals and print infos to user if it was successfull
    get_container_info
    if [ "$CONTAINER_STATUS" == "Up" ]; then
        echo -e "Docker container successfully $wAction with id $CONTAINER_ID"
    else
        return -1
    fi

    return 0
}

#######################################
# Stop the container if it is running
# Globals:
#     CONTAINER_STATUS
#     CONTAINER_ID
# Arguments:
#     None
# Returns:
#     integer: exit code
#######################################
stop_container(){

    # Check if the action can be done
    if [ "$CONTAINER_STATUS" != "Up" ]; then
        echo -e "Container is not running"
        return -1
    fi

    # Stop the container
    docker stop $CONTAINER_ID

    # Print infos to user if it was successfull
    if [ $? -eq 0 ]; then
        echo "The container was successfully stopped"
    else
        return -1
    fi

    return 0
}

#######################################
# Start a shell session on the container
# Globals:
#     CONTAINER_STATUS
#     CONTAINER_ID
# Arguments:
#     None
# Returns:
#     integer: exit code
#######################################
shell_container(){

    # Check if the action can be done
    if [ "$CONTAINER_STATUS" != "Up" ]; then
        echo -e "Container is not running"
        return -1
    fi

    # Create a new shell on the container
    docker exec -t -i $CONTAINER_ID /bin/bash

    # Exit with an error code if there was an error
    if [ $? -ne 0 ]; then
        return -1
    fi

    return 0
}

#######################################
# Remove the daemon container
# Globals:
#     CONTAINER_STATUS
#     CONTAINER_ID
# Arguments:
#     None
# Returns:
#     integer: exit code
#######################################
remove_container(){

    # Check if the action can be done
    if [ "$CONTAINER_STATUS" != "Exited" ]; then
        echo "The container is running and connot be removed. You must stop it first."
        return -1
    fi

    # Remove the container
    docker rm $CONTAINER_ID

    # Print infos to user if it was successfull
    if [ $? -eq 0 ]; then
        echo "The container was successfully removed"
    else
        return -1
    fi

    return 0
}

#######################################
# Start a shell session on the container
# Globals:
#     CONTAINER_STATUS
#     CONTAINER_ID
# Arguments:
#     None
# Returns:
#     integer: exit code
#######################################
status_container(){

    # Print infos to user
    if [ -z $CONTAINER_ID ]; then
        echo "The container doesn't exist."
    else
        echo "The container status is '$CONTAINER_STATUS'"
    fi

    return 0
}




if [ $# -ne 1 ]; then
    echo -e "$PROG_NAME: The program was not started with the right arguments\n"
    print_help
    exit -1
fi

# Update the container informations
get_container_info
wResult=0

case "$1" in

"start")
    start_container
    wResult=$?
    ;;

"stop")
    stop_container
    wResult=$?
    ;;

"shell")
    shell_container
    wResult=$?
    ;;

"remove")
    remove_container
    wResult=$?
    ;;


"status")
    status_container
    wResult=$?
    ;;


*)
    echo -e "$PROG_NAME: unknown argument $1\n"
    print_help
    exit -1
    ;;
esac

exit $wResultId
