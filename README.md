<h1 align="center">Computer liquid cooling interface card</h1>

<p align="center">
  <strong></strong>
  <img src="https://miosix.org/miosixlogohomepage.png" alt="Powered by Miosix" height="18">
</p>


### Introduction

This is the **Project repository for the Advanced Operating Systems course of the Master's Degree Computer Science and 
Engineering at Politecnico di Milano.**

The main goal of the project is to configure a MCU board, ST Microelectronics STM32 F072RBT6DISCOVERY, in order to generate two PWM outputs that control the functioning of a pump-based liquid cooler for PC.

To achieve this behavior, we were to leverage Miosix, an open source embedded Operating System that we used to interact with the board itself.

### Miosix Installation
Miosix kernel is used as submodule as specified in
[official wiki page](https://miosix.org/wiki/index.php?title=Miosix_and_git_workflow).
In order to build the project, first clone the repository. Then move use the command
>git submodule init

>git submodule udpate


### Access the board output
In order to access what the board prints on _screen_, after connecting the board to the computer, you should use the command:
> screen /dev/ttyUSB0 115200

You can then exit _screen_ by pressing the keys combination Ctrl+A -> K -> Y.