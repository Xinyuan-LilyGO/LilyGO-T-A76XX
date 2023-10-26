
## Some simple AT instructions

| command      | explain                              | Response     |
| ------------ | ------------------------------------ | ------------ |
| AT           | AT test instruction                  | OK           |
| AT+SIMCOMATI | version information                  |              |
| AT+CPIN?     | Example Query the SIM card status    | +CPIN: READY |
| AT+CGREG?    | Network Registration Status          | +CGREG: 0,1  |
| AT+CSQ       | Network signal quality query         |              |
| AT+COPS?     | Querying current Carrier Information |              |

Refer to the T-A7670X instruction manual for more [AT instructions](../../datasheet/) 