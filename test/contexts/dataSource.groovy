x = new Properties()
x.putAll([maxActive:"3",
                   maxIdle:"1",
                   username:"damuser",
                   password:"r!VAld0",
                   driverClassName:"org.firebirdsql.jdbc.FBDriver",
                   url:"jdbc:firebirdsql://daVinci//var/firebird/lazerweb.fdb"])

org.apache.commons.dbcp.BasicDataSourceFactory.createDataSource(x)