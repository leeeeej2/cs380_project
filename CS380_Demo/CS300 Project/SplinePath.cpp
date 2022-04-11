#include "SplinePath.h"
const float PI = 4.0f * atan(1.0f);
SplinePath::SplinePath()
{
	scale = glm::vec3(1.f);
	float distance = 10.f / PI;
	glm::vec3 rotate_vector = glm::vec3(0, 1, 0);
	float rotation = 0.f;
	glm::vec3 translate_vector = glm::vec3(distance, 0, 0);
	glm::vec4 transform;

	for (rotation = 0.f; rotation < 2 * PI; rotation += PI / 5)
	{
		transform = glm::rotate(rotation, rotate_vector) * glm::vec4(translate_vector, 1.f);
		points.push_back({ transform.x, transform.y, transform.z });
	}

	for (int i = 0; i < 10; i++)
	{
		optimizedPoints.push_back(points[i]);
		displacement.push_back(0.f);
	}
	current_pts = 0;

	CalculationSplinePoint();
	CalculationTrackPoint();
	CalculationOptimizationPath();

	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &vao2);
	glGenVertexArrays(1, &vao3);
	glGenVertexArrays(1, &optimizedVao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &vbo2);
	glGenBuffers(1, &vbo3);
	glGenBuffers(1, &optimizedVbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * spline_pts.size(), &spline_pts[0].x, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);//spline position 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(vao2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tracks.size(), &tracks[0].x, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);//spline position 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(vao3);
	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tracks2.size(), &tracks2[0].x, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);//spline position 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(optimizedVao);
	glBindBuffer(GL_ARRAY_BUFFER, optimizedVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * optimizedSpline.size(), &optimizedSpline[0].x, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);//spline position 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

SplinePath::~SplinePath()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vbo2);
	glDeleteBuffers(1, &vbo3);
	glDeleteBuffers(1, &optimizedVbo);

	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &vao2);
	glDeleteVertexArrays(1, &vao3);
	glDeleteVertexArrays(1, &optimizedVao);
}

void SplinePath::DrawSplinePoints(ObjectManager* m, glm::mat4 proj, glm::mat4 cam, glm::vec3 c)
{
	int size = points.size();

	for (int i = 0; i < size; ++i)
	{
		m->SetTransforms(ObjectTypeEnum::eCREATE_SPHERE, points[i], scale, 0, scale);
		if (current_pts == i)
		{
			m->DrawObject(ObjectTypeEnum::eCREATE_SPHERE, proj, cam, { 1,0,0 });
		}
		else
		{
			m->DrawObject(ObjectTypeEnum::eCREATE_SPHERE, proj, cam, c);
		}
	}
}

void SplinePath::DrawSplinePath(GLuint* shader, glm::mat4 transform, glm::mat4 proj, glm::mat4 cam, glm::vec3 color)
{
	GLint uniform_transform = glGetUniformLocation(*shader, "transform");
	GLint uniform_camera = glGetUniformLocation(*shader, "cameraMatrix");
	GLint uniform_ndc = glGetUniformLocation(*shader, "ndcMatrix");
	GLint uniform_color = glGetUniformLocation(*shader, "color");

	//set uniform
	glUniformMatrix4fv(uniform_transform, 1, GL_FALSE, &transform[0][0]);
	glUniformMatrix4fv(uniform_camera, 1, GL_FALSE, &cam[0][0]);
	glUniformMatrix4fv(uniform_ndc, 1, GL_FALSE, &proj[0][0]);
	glUniform3f(uniform_color, color.x, color.y, color.z);

	//update vertex
	UpdateBuffers();

	//set vertex buffer
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDrawArrays(GL_LINES, 0, (GLsizei)spline_pts.size());
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void SplinePath::CalculationOptimizationPath()
{
	float trackWidth = 0.5f;

	for (int i = 0; i < points.size(); i++)
	{
		optimizedPoints[i] = points[i];
		displacement[i] = 0.f;
	}

	for (int i = 0; i < iterationNum; i++)
	{
		for (int j = 0; j < optimizedPoints.size(); j++)
		{
			glm::vec3 left = optimizedPoints[(j + 1) % optimizedPoints.size()];
			glm::vec3 middle = optimizedPoints[j];
			glm::vec3 right = optimizedPoints[(j + (optimizedPoints.size() - 1)) % optimizedPoints.size()];

			left = left - middle;
			right = right - middle;

			float leftLength = glm::dot(left, left);
			float rightLength = glm::dot(right, right);

			left = left / leftLength;
			right = right / rightLength;

			glm::vec3 sumVec = left + right;
			float sumLength = glm::dot(sumVec, sumVec);
			sumVec = sumVec / sumLength;

			glm::vec3 gradient = GetSplineGradient(j, true);
			float gradientLength = glm::dot(gradient, gradient);
			gradient = gradient / gradientLength;

			glm::vec3 gradientNormal = glm::vec3(-gradient.z, 0, gradient.x);
			float displace = glm::dot(gradientNormal, sumVec);

			displacement[j] += (displace * 0.003f);
		}

		for (int j = 0; j < points.size(); j++)
		{
			if (displacement[j] >= trackWidth)
			{
				displacement[j] = trackWidth;
			}
			if (displacement[j] <= -trackWidth)
			{
				displacement[j] = -trackWidth;
			}

			glm::vec3 gradient = GetSplineGradient(j, true);
			float gradientLength = sqrtf(gradient.x * gradient.x + gradient.z * gradient.z);
			gradient = gradient / gradientLength;
			glm::vec3 gradientNormal = glm::vec3(-gradient.z, 0, gradient.x);

			optimizedPoints[j] = points[j] + (gradientNormal * displacement[j]);
		}
	}

	optimizedSpline.clear();

	float size = (float)optimizedPoints.size();
	float step = optimizedPoints.size() * 5;
	float alpha = 1 / step;

	glm::vec3 pos;
	glm::vec3 previous_pos;

	previous_pos = GetSplinePoint(0, true);

	for (float t = 0.05f; t < size; t += 0.05f)
	{
		optimizedSpline.push_back(previous_pos);

		pos = GetSplinePoint(t, true);
		previous_pos = pos;

		optimizedSpline.push_back(pos);
	}

}

void SplinePath::DrawTrackPath(GLuint* shader, glm::mat4 transform, glm::mat4 proj, glm::mat4 cam, glm::vec3 color)
{
	GLint uniform_transform = glGetUniformLocation(*shader, "transform");
	GLint uniform_camera = glGetUniformLocation(*shader, "cameraMatrix");
	GLint uniform_ndc = glGetUniformLocation(*shader, "ndcMatrix");
	GLint uniform_color = glGetUniformLocation(*shader, "color");

	//set uniform
	glUniformMatrix4fv(uniform_transform, 1, GL_FALSE, &transform[0][0]);
	glUniformMatrix4fv(uniform_camera, 1, GL_FALSE, &cam[0][0]);
	glUniformMatrix4fv(uniform_ndc, 1, GL_FALSE, &proj[0][0]);
	glUniform3f(uniform_color, color.x, color.y, color.z);

	//update vertex
	UpdateBuffersTrack();

	//set vertex buffer
	glBindVertexArray(vao2);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glDrawArrays(GL_LINES, 0, (GLsizei)tracks.size());
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void SplinePath::DrawTrackPath2(GLuint* shader, glm::mat4 transform, glm::mat4 proj, glm::mat4 cam, glm::vec3 color)
{
	GLint uniform_transform = glGetUniformLocation(*shader, "transform");
	GLint uniform_camera = glGetUniformLocation(*shader, "cameraMatrix");
	GLint uniform_ndc = glGetUniformLocation(*shader, "ndcMatrix");
	GLint uniform_color = glGetUniformLocation(*shader, "color");

	//set uniform
	glUniformMatrix4fv(uniform_transform, 1, GL_FALSE, &transform[0][0]);
	glUniformMatrix4fv(uniform_camera, 1, GL_FALSE, &cam[0][0]);
	glUniformMatrix4fv(uniform_ndc, 1, GL_FALSE, &proj[0][0]);
	glUniform3f(uniform_color, color.x, color.y, color.z);

	//update vertex
	UpdateBuffersTrack2();

	//set vertex buffer
	glBindVertexArray(vao3);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glDrawArrays(GL_LINES, 0, (GLsizei)tracks2.size());
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void SplinePath::DrawOptimizedSplinePoints(GLuint* shader, glm::mat4 transform, glm::mat4 proj, glm::mat4 cam, glm::vec3 color)
{
	GLint uniform_transform = glGetUniformLocation(*shader, "transform");
	GLint uniform_camera = glGetUniformLocation(*shader, "cameraMatrix");
	GLint uniform_ndc = glGetUniformLocation(*shader, "ndcMatrix");
	GLint uniform_color = glGetUniformLocation(*shader, "color");

	//set uniform
	glUniformMatrix4fv(uniform_transform, 1, GL_FALSE, &transform[0][0]);
	glUniformMatrix4fv(uniform_camera, 1, GL_FALSE, &cam[0][0]);
	glUniformMatrix4fv(uniform_ndc, 1, GL_FALSE, &proj[0][0]);
	glUniform3f(uniform_color, color.x, color.y, color.z);

	//update vertex
	UpdateOptimizedBuffers();

	//set vertex buffer
	glBindVertexArray(optimizedVao);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, optimizedVbo);
	glDrawArrays(GL_LINES, 0, (GLsizei)optimizedSpline.size());
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void SplinePath::NextSplinePoints(bool to_left)
{
	if (to_left == true)
	{
		--current_pts;
		if (current_pts < 0)
		{
			current_pts = points.size() - 1;
		}
	}
	else
	{
		++current_pts;
		if (current_pts >= points.size())
		{
			current_pts = 0;
		}
	}
}

void SplinePath::AddSplinePoint()
{
	points.push_back({ 0,0,0 });
	CalculationSplinePoint();
	CalculationTrackPoint();
	CalculationOptimizationPath();

	UpdateBuffers();
	UpdateBuffersTrack();
	UpdateBuffersTrack2();
	UpdateOptimizedBuffers();
}

void SplinePath::DeleteCurrentSplinePoint()
{
	std::vector<glm::vec3>::iterator it = points.begin() + current_pts;
	--current_pts;
	points.erase(it);
	CalculationSplinePoint();
	CalculationTrackPoint();
	CalculationOptimizationPath();

	UpdateBuffers();
	UpdateBuffersTrack();
	UpdateBuffersTrack2();
	UpdateOptimizedBuffers();
}

int SplinePath::GetCurrentPointID()
{
	return current_pts;
}

void SplinePath::MoveCurrentPoints(bool to_front, bool way)
{
	float distance = PI / 360.f;
	if (to_front == true)
	{
		if (way == true)//forward
		{
			points[current_pts].z -= distance;
		}
		else //backward
		{
			points[current_pts].z += distance;
		}
	}
	else
	{
		if (way == true) //left
		{
			points[current_pts].x += distance;
		}
		else //right
		{
			points[current_pts].x -= distance;
		}
	}
	CalculationSplinePoint();
	CalculationTrackPoint();
	CalculationOptimizationPath();

	UpdateBuffers();
	UpdateBuffersTrack();
	UpdateBuffersTrack2();
	UpdateOptimizedBuffers();
}

glm::vec3 SplinePath::GetSplinePoint(float t, bool IsOptimized)
{
	int p0, p1, p2, p3;
	int size = points.size();
	if (IsOptimized)
	{
		size = optimizedPoints.size();
	}
	p1 = ((int)t) % size;
	p2 = (p1 + 1) % size;
	p3 = (p1 + 2) % size;

	if (p1 >= 1)
	{
		p0 = p1 - 1;
	}
	else
	{
		p0 = size - 1;
	}

	t = t - (int)t;

	float tt = t * t;
	float ttt = tt * t;

	float q1 = -ttt + 2.f * tt - t;
	float q2 = 3.f * ttt - 5.f * tt + 2.f;
	float q3 = -3.f * ttt + 4.f * tt + t;
	float q4 = ttt - tt;

	float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
	float tz = 0.5f * (points[p0].z * q1 + points[p1].z * q2 + points[p2].z * q3 + points[p3].z * q4);
	if (IsOptimized)
	{
		tx = 0.5f * (optimizedPoints[p0].x * q1 + optimizedPoints[p1].x * q2 + optimizedPoints[p2].x * q3 + optimizedPoints[p3].x * q4);
		tz = 0.5f * (optimizedPoints[p0].z * q1 + optimizedPoints[p1].z * q2 + optimizedPoints[p2].z * q3 + optimizedPoints[p3].z * q4);
	}
	return { tx, 0, tz };
}

glm::vec3 SplinePath::GetSplineGradient(float t, bool IsOptimized)
{
	int p0, p1, p2, p3;
	int size = points.size();
	if (IsOptimized)
	{
		size = optimizedPoints.size();
	}

	p1 = ((int)t) % size;
	p2 = (p1 + 1) % size;
	p3 = (p1 + 2) % size;

	if (p1 >= 1)
	{
		p0 = p1 - 1;
	}
	else
	{
		p0 = size - 1;
	}

	t = t - (int)t;

	float tt = t * t;
	float ttt = tt * t;

	float q1 = -3.0f * tt + 4.0f * t - 1.0f; 		// -3t^2 + 4t - 1
	float q2 = 9.0f * tt - 10.0f * t;				// 9t^2 - 10t 
	float q3 = -9.0f * tt + 8.0f * t + 1.0f;		// -9t^2 + 8t + 1
	float q4 = 3.0f * tt - 2.0f * t;				// 3t^2 - 2t

	float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
	float tz = 0.5f * (points[p0].z * q1 + points[p1].z * q2 + points[p2].z * q3 + points[p3].z * q4);
	if (IsOptimized)
	{
		tx = 0.5f * (optimizedPoints[p0].x * q1 + optimizedPoints[p1].x * q2 + optimizedPoints[p2].x * q3 + optimizedPoints[p3].x * q4);
		tz = 0.5f * (optimizedPoints[p0].z * q1 + optimizedPoints[p1].z * q2 + optimizedPoints[p2].z * q3 + optimizedPoints[p3].z * q4);
	}
	return { tx, 0, tz };
}

void SplinePath::CalculationSplinePoint()
{
	spline_pts.clear();
	float size = (float)points.size();
	float step = points.size() * 5;
	float alpha = 1 / step;

	glm::vec3 pos;
	glm::vec3 previous_pos;

	previous_pos = GetSplinePoint(0);

	for (float t = 0.05f; t < size; t += 0.05f)
	{
		spline_pts.push_back(previous_pos);

		pos = GetSplinePoint(t);
		previous_pos = pos;

		spline_pts.push_back(pos);
	}

}

void SplinePath::CalculationTrackPoint()
{
	tracks.clear();
	tracks2.clear();
	float size = (float)points.size();
	float step = points.size() * 5;
	float alpha = 1 / step;

	glm::vec3 pos;
	glm::vec3 previous_pos;
	glm::vec3 previous_pos2;
	glm::vec3 gPos;
	glm::vec3 previous_gPos;

	float trackWidth = 0.5f;

	previous_pos = GetSplinePoint(0);
	previous_pos2 = GetSplinePoint(0);
	previous_gPos = GetSplineGradient(0);

	float gLen;

	for (float t = 0.f; t < size; t += 0.05f)
	{
		tracks.push_back(previous_pos);
		tracks2.push_back(previous_pos2);

		pos = GetSplinePoint(t);
		gPos = GetSplineGradient(t);

		gLen = sqrtf(gPos.x * gPos.x + gPos.z * gPos.z);

		float x = pos.x + trackWidth * (-gPos.z / gLen);
		float z = pos.z + trackWidth * (gPos.x / gLen);

		float x2 = pos.x - trackWidth * (-gPos.z / gLen);
		float z2 = pos.z - trackWidth * (gPos.x / gLen);

		previous_pos = { x,0.f,z };
		previous_pos2 = { x2,0.f,z2 };

		previous_gPos = gPos;

		tracks.push_back({ x,0.f,z });
		tracks2.push_back({ x2,0.f,z2 });
	}

}

void SplinePath::UpdateBuffers()
{


	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * spline_pts.size(), &spline_pts[0]);


}

void SplinePath::UpdateBuffersTrack()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * tracks.size(), &tracks[0]);
}

void SplinePath::UpdateBuffersTrack2()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * tracks2.size(), &tracks2[0]);
}

void SplinePath::UpdateOptimizedBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, optimizedVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * optimizedSpline.size(), &optimizedSpline[0]);
}

void SplinePath::UpdateCar(float delta)
{
	car += 0.2f * delta;
}

void SplinePath::MovingCar()
{
	glm::vec3 splinePoint = GetSplinePoint(car, true);
	glm::vec3 splineGradient = GetSplineGradient(car, true);

	coordinate = glm::vec3(splinePoint.x, 0, splinePoint.z);
	rotation = atan2f(splineGradient.z, splineGradient.x);
}

void SplinePath::IncreaseIteration()
{
	iterationNum++;
	CalculationOptimizationPath();
	UpdateOptimizedBuffers();
}
