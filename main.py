# For this section of the project, I watched a tutorial by the YouTuber "Analytic with Adam"
# URL - https://www.youtube.com/watch?v=0FtcHjI5lmw to get a solid understanding on how the YouTube API requires call
# requests for comments. I did additional research in the actual GitHub repo for the Google api python client,
# URL for GitHub Repo - https://github.com/googleapis/google-api-python-client#readme
# URL for actual YouTube API Docs - https://developers.google.com/youtube/v3/docs
# I also made use of a Geeks for geeks article on writing to csv files in python
# URL - https://www.geeksforgeeks.org/writing-csv-files-in-python/#
from googleapiclient.discovery import build
import csv
import demoji
# need to install demoji and googleapiclient

# this function finds and removes all emojis in a string, I use it because you cannot write emojis to a csv file
def clear_emojis(string):
    dem = demoji.findall(string)
    for item in dem.keys():
        string = string.replace(item, '')
    return string

# This is my personal Youtube API Key, please be careful what videos you use so that I don't run out of API tokens
api_key = 'AIzaSyD7RmH3pB12AhfaYUfo39gmoJT7gBpDLac'


def video_data(video_id):
    # empty list for storing reply
    replies = []
    # initializing statistic variables
    title = ""
    viewcount = 0
    totLikes = 0
    numComments = 0

    # creating youtube resource object
    youtube = build('youtube', 'v3',
                    developerKey=api_key)

    # getting general video statistics,
    video_stats = youtube.videos().list(
        part="snippet,statistics",
        id=video_id
    ).execute()

    # extracting statistics from the video_stats object
    for item in video_stats['items']:
        title = item['snippet']['title']
        viewcount = item['statistics']['viewCount']
        totLikes = item['statistics']['likeCount']
        numComments = item['statistics']['commentCount']

    print(title, " ", viewcount, " ", totLikes, " ", numComments)

    # retrieve youtube video results
    video_response = youtube.commentThreads().list(
        part='snippet,replies',
        videoId=video_id,
        maxResults=100
    ).execute()

    filename = "test.csv"

    # first row of csv file
    statsRows = [[title, viewcount, totLikes, numComments, ]]

    # writing to csv file
    with open(filename, 'w') as csvfile:
        # creating a csv writer object
        csvwriter = csv.writer(csvfile)

        # writing the data rows
        csvwriter.writerows(statsRows)

        # iterate video response
        while video_response:

            # extracting required info
            # from each result object
            for item in video_response['items']:

                # Extracting comments
                comment = item['snippet']['topLevelComment']['snippet']['textDisplay']

                clear_emojis(comment)

                commentLikes = item['snippet']['topLevelComment']['snippet']['likeCount']

                # counting number of reply of comment
                replycount = item['snippet']['totalReplyCount']

                # if reply is there
                if replycount > 0:

                    # iterate through all reply
                    for reply in item['replies']['comments']:
                        # Extract reply
                        reply = reply['snippet']['textDisplay']
                        cleanReply = clear_emojis(reply)

                        # Store reply is list
                        replies.append(cleanReply)

                # print comment with list of reply
                print(clear_emojis(comment), commentLikes, replies, end='\n\n')

                dataRows = [[clear_emojis(comment), commentLikes, replies,]]


                # creating a csv writer object
                csvwriter = csv.writer(csvfile)

                # writing the data rows
                csvwriter.writerows(dataRows)

                # empty reply list
                replies = []

            # Again repeat
            if 'nextPageToken' in video_response:
                video_response = youtube.commentThreads().list(
                    part='snippet,replies',
                    videoId=video_id,
                    maxResults=100,
                    pageToken=video_response['nextPageToken']
                ).execute()
            else:
                break



# Enter video id
video_id = "yCZceKHtnWE"

# Call function
video_data(video_id)
