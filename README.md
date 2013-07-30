OwNet
=========

OwNet is a project that aims to help schools and individuals with poor
Internet connectivity get better access to the Web.
The main part of OwNet is a proxy application that employs caching and
prefetching of websites to speed up their load times.
The website cache is stored on the users' computers and is accessible from all
computers running OwNet on the local network.

OwNet also provides a social portal for students to share links to
websites, create groups and chat on the local network.

## Architecture

OwNet is a Qt proxy application that runs on Windows, Mac OS X and Linux.
Information about the available cache and data shared by users are synchronized
over the local network in a peer-to-peer fashion.
Each instance of OwNet contains a sync journal that is being compared with other
instances during synchronization.
Shared data can also be synced with students in the same workgroup over the
Internet using a central service (not part of this repository).

## Deployment and state of the project

We have participated with NGOs from Slovakia to deploy OwNet in schools in Kenya.
We were successful in installing OwNet in several schools, however further work
is required to include OwNet in their teaching process.

The development of OwNet has been suspended.
The available source code contains several known bugs and issues that still need
to be resolved.

## Team

This project has been developed on the Faculty of Informatics and Information
Technologies STU in Bratislava.

In the development of the project participated:

- Karol Balko
- Michal Barla (@michalbarla)
- Ľuboš Demovič (@LubosDemovic)
- Michal Dorner
- Martin Konôpka (@MartinKonopka)
- Marek Láni
- Martin Lipták (@martinliptak)
- Karol Rástočný
- Andrea Šteňová
- Matúš Tomlein (@matus_tomlein)

## More information

For more information, visit the [project homepage](http://ownet.fiit.stuba.sk/).