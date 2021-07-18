[![LinkedIn][linkedin-shield]][linkedin-url]
<!--
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]


[![Github][github-shield]][github.com/zoumson?tab=repositories]
[![Stack Overflow][stackoverflow-shield]][stackoverflow.com/users/11175375/adam]
[![Leetcode][leetcode-shield]][eetcode.com/Hard_Code/]
-->
## Multi-Client single server using multi-threading
![image](https://user-images.githubusercontent.com/38358621/120076656-41d75780-c0d9-11eb-97fc-7f56e4cfa9bf.png)




<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

<!-- [![Product Name Screen Shot][product-screenshot]](https://example.com) -->

The goal of this mini-project is to develop a client server application
allowing the stock management of a company. 

* Each customer connects to the server, enter the name of the product (s) ordered, the quantities,
and his name. 
* The server displays the price of the order, and creates a file
whose name is unique (for example created according to the date) which
contains the order data. 
* The database is stored in a text file in which each line contains a product name (without
space), and a unit price. 
* Communication between customers and server is done through sockets. 
* The processing by the server of Customer orders are done through threads; at each request
* client is associated with a thread. 
* Customer order history must be saved in a text file.

A list of commonly used resources that I find helpful are listed in the acknowledgements.
### Built With


* [boost](https://boost.org/)
* [cmake](https://cmake.org/)
* [gnu](https://www.gnu.org/)



<!-- GETTING STARTED -->
## Getting Started

This is an sample code of how to implement a single server mutilple clients communication mutli-threading.
<br>
To get a local copy up and running follow these simple steps.

## File Structure

### Folders

* [include/](include/): c++ declarations.
* [resource/](resource/): images.
* [src/](src/): c++ definitions.


### Entire Files Structure 

```
.
├── CMakeLists.txt
├── include
├── README.md
├── resource
│   └── price.txt
└── src
    ├── client.c
    └── server.c

3 directories, 5 files

```

### Prerequisites

This is an example of how to list things you need to use the software and how to install them.
* cmake
  ```sh
  sudo apt-get install cmake
  ```

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/zoumson/MuliThreadingManageCompanyProduct.git
   ```
2. Go to the project directory source
   ```sh
   cd MuliThreadingManageCompanyProduct
   ```
3. Create empty directories 
   ```sh
   mkdir build &&  mkdir bin 
   ```
5. Generate the executables and move them to `bin`
   ```sh
   cd build && cmake .. && make -j4 && cd ..
   ```

<!-- USAGE EXAMPLES -->
## Usage
Command line arguments
<br>
1.  Sever side 
```
./bin/server -h
```
```
./Server [options]
[options]
[M]	-m [Server Max Client]			Server maximum clients.
[M]	-p [Server Port Number]		Server to connect Port Number.
[O]	-h 				Show usage
```
* Dafault server port number: random available port from host, default maximum client is 5
   ```sh
   ./server 
   ```
 * Self defined server port number and maximum clients: myPort, myClients 
   ```sh
   ./server -p myPort -m myClients
   ```
 * Server log for each client: `Client[1]_2021_07_19_02_40_50.txt`
* Server log for each client
```
[2021-07-19 02:40:53][Client 1] Name [zouma]
[2021-07-19 02:41:00][Client 1] number of product type is [2]
[2021-07-19 02:41:04][Client 1] Product name is [mango]
[2021-07-19 02:41:10][Client 1] Product quantity is [10]
[2021-07-19 02:41:10][Client 1]Product [mango] Quantity[10] Unit Price[10.000000] Total Price[100.000000]
[2021-07-19 02:41:16][Client 1] Product name is [you]
[2021-07-19 02:41:21][Client 1] Product quantity is [2]
```
* Server historic for all client: `history.txt`
 ```
[2021-07-19 02:40:53][Client 1] Name is [zouma]
[2021-07-19 02:41:00][Client 1] number of product type is [2]
[2021-07-19 02:41:04][Client 1] Product name is [mango]
[2021-07-19 02:41:10][Client 1] Product quantity is [10]
[2021-07-19 02:41:10][Client 1]Product [mango] Quantity[10] Unit Price[10.000000] Total Price[100.000000]
[2021-07-19 02:41:16][Client 1] Product name is [you]
[2021-07-19 02:41:21][Client 1] Product quantity is [2]
[2021-07-19 02:41:21][Client 1]Product [you] does not exit in database
[2021-07-19 02:44:13][Client 1] Name is [Ali]
[2021-07-19 02:44:15][Client 1] number of product type is [1]
[2021-07-19 02:44:19][Client 1] Product name is [tomato]
[2021-07-19 02:44:25][Client 1] Product quantity is [20]
[2021-07-19 02:44:25][Client 1]Product [tomato] Quantity[20] Unit Price[5.000000] Total Price[100.000000]
 ```
2.  Client side 

```
/bin/client -h 
```

```
./Client [options]
[options]
[M]	-i [Server IP]			Server to connect to digit IP address.
[M]	-p [Server Port Number]		Server to connect to Port Number.
[O]	-h 				Show usage

* Sample client log( `/bin/client -p 50000 -i 192.168.0.185`)
```
Client initializing ...
Creating a client socket ...
A client socket created
Client initialized
Connecting to a server ...
A client connected to a server
Please insert your name:
zouma
Customer name is [zouma]
A Client sending a request ...
Request sent to a server
Please insert the number of product type
2
Total number of product type is [2]
A Client sending a request ...
Request sent to a server
Insert Product name:
mango
Product name is [mango]
A Client sending a request ...
Request sent to a server
Insert Product[mango] quantity:
10
A Client sending a request ...
Request sent to a server
Product[mango] quantity is [10]
Insert Product name:
orange
Product name is [orange]
A Client sending a request ...
Request sent to a server
Insert Product[orange] quantity:
2
A Client sending a request ...
Request sent to a server
Product[orange] quantity is [2]
A Client closing a connexion ...
Connexion with a server closed
```
3. Back to the initial file structure configuration
 ```sh
  rm -r bin build 
 ```
      
   
<!-- ROADMAP -->
## Roadmap

All the headers files are well docummented, read through the comments



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

Adama Zouma - <!-- [@your_twitter](https://twitter.com/your_username) -->- stargue49@gmail.com

Project Link: [https://github.com/zoumson/MuliThreadingManageCompanyProduct(https://github.com/zoumson/MuliThreadingManageCompanyProduct.git)



<!-- ACKNOWLEDGEMENTS -->

## Acknowledgements
* [Google](https://www.google.com/)
* [Stack Overflow](https://stackoverflow.com/)
* [Github](https://github.com/)
* [Git Concept from Telusko](https://www.youtube.com/watch?v=WbwIoQYP6no)
* [Git comands from The Coding Train](https://www.youtube.com/watch?v=yXT1ElMEkW8)
* [Git basics from Programming with Mosh ](https://www.youtube.com/watch?v=8JJ101D3knE)
* [Oh My ZSH](https://medium.com/@ivanaugustobd/your-terminal-can-be-much-much-more-productive-5256424658e8)
* [nerd-fonts](https://github.com/ryanoasis/nerd-fonts/tree/master/patched-fonts)
* [powerlevel9k/powerlevel9](https://github.com/Powerlevel9k/powerlevel9k#available-prompt-segments)





<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: images/screenshot.png
