# Pullfinger

A GTK 3 based finger client implementation.

# Explanation
# The Finger Protocol

Imagine the Finger protocol as a virtual way to introduce one computer to another. It’s like asking someone, “What are you up to right now?” The Finger protocol lets one computer inquire about the current activities, contact information, or even the name of another computer. However, here’s the catch: the Finger protocol transmits messages in plain text. It’s like writing a personal message on a postcard – anyone who encounters it can read it. This lack of security means the Finger protocol isn’t suitable for sharing sensitive information. Think of it as sending a postcard with your plans for the day written on it, allowing anyone to see what you’re up to.

# Comparing Finger with HTTPS

Let’s put the Finger protocol in perspective by comparing it with the modern HTTPS protocol:

    Finger Protocol:
        Origin: Developed in the early days of the internet when security concerns were less prevalent.
        Message Security: Transmits messages in plain text, making them vulnerable to snooping and eavesdropping.
        Privacy Measures: Lacks robust security mechanisms, making it unsuitable for handling confidential or private information.
        Usage: Used primarily for simple information retrieval between computers in the early internet era.

    HTTPS Protocol:
        Origin: Evolved to address the need for secure communication in the modern digital landscape.
        Message Security: Wraps messages in layers of encryption, ensuring that only intended recipients can decipher them.
        Security Measures: Utilizes digital certificates to verify the identity of servers and websites, enhancing trust and security.
        Usage: Widely employed for secure online transactions, protecting login credentials, and safeguarding sensitive data.

# Constructing a GUI Finger Client

In the digital realm of 2023, where modern protocols and secure connections are the norm, the idea of creating a Finger client might seem unconventional. However, the endeavor serves as a window into the past and a reminder of how far internet communication has come. Building a Finger client today isn’t about practicality; it’s about understanding the origins of online interaction, comparing it to modern security-focused protocols like HTTPS, and appreciating the historical context that has shaped our digital landscape. By delving into the development of a Finger client, we explore the evolution of networking and gain insights into the security measures that are now integral to our online interactions.

# Installation guide