# s3fs-cryptopp-fuse-s3
Implementation of AES-CBC (Advanced Encryption Standard – Cipher Block Chaining) File Encryption &amp; Decryption using Amazon’s S3 cloud storage.

This project aimed to implement AES-CBC File Encryption & Decryption using Amazon S3 whenever a file is created, edited, or placed into the local mount location.
The file is immediately encrypted and uploaded to the Amazon S3 bucket.

All files in the local file system are to be automatically decrypted and the user can and the user can operate and use these files without explicitly having to call for any kind of decryption. 
From the web interface of the Amazon S3 bucket, the file would remain encrypted.
Whenever a file is moved from the online bucket to the local folder, the file in question would immediately become decrypted using a local encryption/decryption key stored somewhere on the user’s computer

This project uses:

*Amazon S3

*s3fs

*Crypto++

*FUSE

A list of installation instructions and final report example can be found in README and Nabors_Jonathan_Final_Report_CSC4420.pdf
