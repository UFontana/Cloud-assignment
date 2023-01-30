import React, { useState, useEffect } from 'react';
import { useParams, Link, useNavigate } from "react-router-dom";
import Button from 'react-bootstrap/Button';
import InputGroup from 'react-bootstrap/InputGroup';
import FormControl from 'react-bootstrap/FormControl';
import deleteImg from './trash.png';
import 'bootstrap/dist/css/bootstrap.css';

import './User.css';

function User(props) {
    const { userId, userName } = useParams();
    const [ newPostText, setNewPostText ] = useState('');
    const [ newPostId, setNewPostId ] = useState('');
    let navigate = useNavigate();

    useEffect(() => {
        props.updatePosts(userId);
    }, [userId]);

    function handleTextChange(event) {
        setNewPostText(event.target.value);
    }

    function handleIdChange(event) {
        setNewPostId(event.target.value);
    }

    function handlePostRegister() {
        props.post(newPostId, newPostText, userId);
        setNewPostId('');
        setNewPostText('');        
    }

    function GDPRDelete(event) {
        props.deleteUser(userId);
        props.logout();
        navigate("/", { replace: true });
    }

    if (userId === props.signedInUserId) {
        return (
            <div className="User">
              <div className="UserButtons">
                  {<Link to="/users" style={{width:"100%"}}><Button variant="primary" style={{width:"100%"}}>Home</Button></Link>}
                  {/*<Button variant="danger" onClick={GDPRDelete}>Home</Button>                  */}
              </div>
              <h1>Your Timeline</h1>
              <div className="CreatePost">
                <InputGroup>
                    <FormControl
                        placeholder="Post ID"
                        aria-label="Post ID"
                        aria-describedby="createPost"
                        value={newPostId} 
                        onChange={handleIdChange}
                    />
                    <FormControl
                        placeholder="What's on your mind?"
                        aria-label="What's on your mind?"
                        aria-describedby="createPost"
                        value={newPostText} 
                        onChange={handleTextChange}
                    />
                    <Button
                        variant="primary" 
                        id="createPost"
                        onClick={handlePostRegister}>Post</Button>
                </InputGroup>
              </div>
              
              <div className="User">
                    <div className="Posts">
                        {props.posts.length === 0 && 
                            <div className="floatContainer">
                                <p>No posts yet!</p>
                            </div>
                        }
                        {props.posts.length > 0 && props.posts.map(post => (
                            <div className="floatContainer" key={post.postId}>
                                <div className="Post">
                                    <p>{post.postContent}</p><i className="idText" style={{marginBottom: "5px"}}>{post.postId}, {post.shard}</i>
                                </div>
                                <div className="deleteBtn">
                                    <Button 
                                    variant="danger" 
                                    onClick={() => props.deletePostAndUpdate(userId, post.postId)}
                                    >
                                        <img src={deleteImg} alt="trash can"/>
                                    </Button>
                                </div> 
                            </div>
                        ))}
                    </div>
                </div>
            </div>
          );
    } else {
        return (
            <div className="User">
                <div className="UserButtons">
                    <Link to="/users"><Button variant="light">Back</Button></Link>                    
                </div>
                <h1>{userName}'s Timeline</h1>
                <div className="Posts">
                    {props.posts.length === 0 && 
                        <div className="floatContainer">
                            <p>No posts yet!</p>
                        </div>
                    }
                    {props.posts.length > 0 && props.posts.map(post => (
                        <div className="Post" key={post.postId}>
                            <p>{post.postContent}</p><i className="idText" style={{marginBottom: "5px"}}>{post.postId}, {post.shard}</i>
                        </div>
                    ))}                    
                </div>
            </div>
          );
    }
    
  }

export default User;