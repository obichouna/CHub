# CHub
## Inbar Pe'er, Dasha Shifrina, Angelica Zverovich, Othman Bichouna (Period 5)
## Description: 
  Our very primitive bug filled not functioning version of GitHub which we named CHub. 
## Instructions on how to use your project:
   Run make. 
   Set up a server running on one computer and determine its IP address.
   Set up a client running on another computer and enter the server's IP address. 
   Type in the github command and a file name seperated by a space. 
## Features:
  The client is able to establish a connection with the server and they are able to exchange information between one another. In order to create a repo on the server, client must type in create, followed by the server name(ex: create awesome-repo). In order to pull a file from the server, client must type in clone, followed by the file name(ex: clone awesome-file.txt). Pulling file is supposed to be a primitive version of pulling directories.  
   
## Attempted:
  <p> We tried to implement a username system but decided to focus on the file sending first as we felt that was more important. Although we started setting up the basics for a login system we spent all of our time trying to get the push/pull to work and never implemented the username system.  </p>
  <p> We also tried to work on commits by creating a seperate folder that would push files in a FIFO order. Monty worked on that in a seperate file but when we started having serious problems with the basic push/pull he dropped that to help work on push/pull. </p>
   <p> We also tried to create a text editor because we were overly ambitious at the begining of the project and thought we could link up a texteditor and create files using that and then connect them to the CHub we'd created. We thought we could one up github (clearly we were not even close to succeeding). </p>
## Bugs & Unresolved Issues:
 <p> After working on push and pull for most of the last day, we ended up breaking our code. We can't find the commit that had the previously working version and we tried for most of the remaining night to go back to that starting point but we can't find it anywhere. To see our efforts, you can check either Angelica's or Dasha's or Inbar's accounts to see all of the new repos we made to test through old code to just find everything that worked before. Our version before was able to proficiently create repos and primitively pull files from the server side.  </p>
<p> When we chose our project we hadn't yet learned networking in class and we thought that it'd be very simple. We didn't realzie how difficult it would be to send files between comptuers and we didn't realize that there were very few online resources in C for learning how. We began our project by thinking we needed to first create a text editor before anything else. After sinking a lot of time into that we realized we didn't need the text editor so we scrapped it and started compiling onlne resources for how to send and recieve files. Inbar spent a lot of time trying to create functions that sent and recieved files successfully. </p>
 <p> We tried to put everything together towards the end of the project and that's when things really fell apart. The code for sending and recieiving files had to be integrated into the source code and in that process we messed a lot of things up. Sometimes when we ran it things worked, other times they didn't and we couldn't figure out why. Our code got increasingly messy and Inbar and Monty tried to restart the code in a seperate repo but it didn't work any better. Dasha and Angelica spent hours making small changes trying to fix things to no avail. </p>
 <p> This is not a project any of us are proud of. We're very sorry. We've never worked harder on something only to accomplish so little. We wish that our project better showed off our programming skills and all that we learned in Systems this year, but we were overly ambitious and our understanding of networking was not as strong as we'd hoped. </p>
