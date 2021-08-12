import React,{useEffect,useState} from 'react';
import './App.css';
import axios from 'axios'
import { Card, Icon, Image, Button } from 'semantic-ui-react'


function App() {
  const [hotelRooms,setHotelRooms] = useState([])

  useEffect(()=>{
    axios.get("https://iot-project-ic.herokuapp.com/device/")
      .then(function (response) {
        // handle success
        console.log(response);
        setHotelRooms(response)

      })
      .catch(function (error) {
        // handle error
        console.log(error);
      })
  },[])

  const sendToDevice = (e,url,led)=>{
    e.preventDefault()
    const ledTodevice = !led
    const ledState = {"ledState":ledTodevice}
    axios.post(url,ledState)
  }
  return (
    <div className="App">
      <header className="App-header">
        <title>Hotel Transylvania light management system</title>
      </header>
      <div className="App-body">
        {
          hotelRooms.length>0?
          <div>
            {
              hotelRooms.map((hotel)=>{
                return <div>
                  <Card>
                    <Image src='/images/avatar/large/matthew.png' wrapped ui={false} />
                    <Card.Content>
                      <Card.Header>{`Room No. ${hotel.roomNumber.number}`}</Card.Header>
                      <Card.Meta>
                        <span className='date'>{`Last activity ${hotel.lastActivity}`}</span>
                      </Card.Meta>
                      <Card.Description>
                        
                      </Card.Description>
                    </Card.Content>
                    <Card.Content extra>
                      
                        <Icon name="circle thin" color={hotel.ledState?"green":"grey"}>
                        Lighting State
                        </Icon>
                       <span>
                        <Button primary onClick={sendToDevice(`http://${hotel.ip_address}/device`,hotel.ledState)}><Icon name="lightbulb">
                          Admin
                        </Icon></Button>
                       </span>
                    </Card.Content>
                  </Card>
                </div>
              })
            }
          </div>:"Loading......"
        }
      </div>

    </div>
  );
}

export default App;
