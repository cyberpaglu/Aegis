if tcp.syn then alert("TCP SYN scan detected")
if dns.query then alert("Malicious DNS query")
if http.url then alert("Suspicious HTTP URL")

