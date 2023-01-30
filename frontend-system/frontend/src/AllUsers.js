import React from 'react';
import { Link } from "react-router-dom";
import 'bootstrap/dist/css/bootstrap.css'
import person from './person.png'
import Button from 'react-bootstrap/Button'
import './User.css';

function AllUsers(props) {

    return (
        <div className="User">
            <Logout logout={props.logout}/>
            <h1>Welcome, {props.userName}.</h1>
            <div className="Posts text-left">
                <h2 style={{ margin: "10px" }}>All Users</h2>
                <List list={props.list} />
            </div>
        </div>
    )
}

const List = ({ list }) => (
    <div className="UserCards">
        {list.map((user) => (
            <div className="card UserCard" key={user.userId}>
                <Link to={"/user/" + user.userId + "/" + user.userName}>
                    <img className="card-img-top" src={person} alt="Default profile pic"/>
                    <div className="UserCardText" key={user.userId}>
                        <div className="UserCardTextTitle">
                            <h3>{user.userName} </h3>
                            <i style={{ marginBottom: "0.75rem" }} className="idText">{user.userId}, {user.shard}</i>
                        </div>
                        <Button variant="light" style={{ width: "100%" }}>See Posts</Button>
                    </div>
                </Link>
            </div>
            
        ))}
    </div>
);

const Logout = ({ logout }) => (
    <Link to="/">
        <div className="UserButtons">
            <Button 
                variant="primary"
                type="button" 
                onClick={logout}
                style={{ width: "100%" }}
            >
                Logout
            </Button>
        </div>
    </Link>
)

export default AllUsers;
