import React, { useState } from 'react'
import { Navigate } from "react-router-dom";
import 'bootstrap/dist/css/bootstrap.css'

import logo from './logo.png'
import Button from 'react-bootstrap/Button'
import InputGroup from 'react-bootstrap/InputGroup'
import FormControl from 'react-bootstrap/FormControl'
import Container from 'react-bootstrap/Container'
import Col from 'react-bootstrap/Col'
import Row from 'react-bootstrap/Row'

import './Login.css';

const Login = (props) => {
  const [name, setName] = useState('');
  const [signUpName, setSignUpName] = useState('');
  const [id, setId] = useState('');

  function handleNameChange(event) {
    setName(event.target.value);
  }

  function handleSignUpNameChange(event) {
    setSignUpName(event.target.value);
  }

  function handleIdChange(event) {
    setId(event.target.value);
  }

  function handleSignInRegister() {
    props.signIn(name)
  }

  function handleSignUpRegister() {
    props.signUp(signUpName, id)
  }

  if (props.redirect) {
      return <Navigate to={props.redirect} />
  }  
  else {
      return (
        <div>
            <Container className="homepage">
            <Row>
                <Col className="center">
                    <img src={logo} alt="EURECOM logo" style={{width: "70%", height: "auto"}} />
                    <div style={{ height: "20px"}}/>
                    <h1 style={{ textAlign: 'center', fontFamily: "Arial", fontStyle: "italic", fontSize: "48pt"}}>E U R E B O O K</h1>
                    <div style={{ height: "20px"}}/>
                    <div>
                      <EnterUsername
                          name={name}
                          onChange={handleNameChange}
                          onRegister={handleSignInRegister}
                      />
                      <SignUp
                          name={signUpName}
                          id={id}
                          handleNameChange={handleSignUpNameChange}
                          handleIdChange={handleIdChange}
                          onRegister={handleSignUpRegister}
                      />
                    </div>
                    <div style={{ height: '5vw' }}/>
                </Col>
            </Row>
            </Container>
        </div>
    );
  }
};

const EnterUsername = ({ name, onChange, onRegister }) => (
  <div className='EnterUsername'>
    <InputGroup type="text">
        <FormControl
            type="text" 
            value={name} 
            onChange={onChange} 
            placeholder={"Sign in with user ID"}
            aria-label={"userId"}
            aria-describedby={"enterUserId"}
        />
        <Button 
            variant="primary" 
            id="createPost"
            type="button" 
            onClick={onRegister}
        >
        Log In
        </Button>
    </InputGroup>
  </div>
);

const SignUp = ({ signUpName, id, handleNameChange, handleIdChange, onRegister }) => (
    <div className='EnterUsername'>
    <InputGroup type="text">
        <FormControl
            type="text" 
            value={signUpName} 
            onChange={handleNameChange} 
            placeholder={"Sign up with username"}
            aria-label={"Username"}
            aria-describedby={"enterUsername"}
        />
        <FormControl
            type="text" 
            value={id} 
            onChange={handleIdChange} 
            placeholder={"Sign up with ID"}
            aria-label={"Id"}
            aria-describedby={"enterId"}
        />
        <Button 
            variant="primary" 
            id="createPost"
            type="button" 
            onClick={onRegister}
        >
        Sign Up
        </Button>
    </InputGroup>
  </div>
)


export default Login;
