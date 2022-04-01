firewall-cmd --permanent --add-port=3260/tcp
firewall-cmd --permanent --add-port=5672/tcp
firewall-cmd --permanent --add-port=80/tcp
firewall-cmd --permanent --add-port=21/tcp
firewall-cmd --permanent --add-port=22/tcp
firewall-cmd --permanent --add-port=23/tcp
firewall-cmd --permanent --add-port=9292/tcp
firewall-cmd --permanent --add-port=9696/tcp
firewall-cmd --reload 
